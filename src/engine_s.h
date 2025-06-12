static constexpr size_t g_engine_cycles_per_frame = 512;

struct engine_s
{
    struct node_s* node;
    size_t size;
    struct crankshaft_s crankshaft;
    struct flywheel_s flywheel;
    struct starter_s starter;
};

struct engine_time_s
{
    uint64_t fluids_time_ms;
    uint64_t thermo_time_ms;
    uint64_t tbd_time_ms;
    uint64_t kinematics_time_ms;
    uint64_t (*get_ms)();
};

static void
normalize_engine(struct engine_s* self)
{
    for(size_t i = 0; i < self->size; i++)
    {
        normalize_node(&self->node[i]);
    }
}

static void
rig_engine_pistons(struct engine_s* self)
{
    for(size_t i = 0; i < self->size; i++)
    {
        struct node_s* node = &self->node[i];
        if(node->type == is_piston)
        {
            rig_piston(&node->as.piston, &self->crankshaft);
        }
    }
}

static void
flow_engine(struct engine_s* self, struct sampler_s* sampler)
{
    for(size_t i = 0; i < self->size; i++)
    {
        struct node_s* x = &self->node[i];
        for(size_t next, j = 0; (next = x->next[j]); j++)
        {
            struct node_s* y = &self->node[next];
            struct nozzle_flow_s nozzle_flow = flow(&x->as.chamber, &y->as.chamber);
            if(x->is_selected)
            {
                sample_channel(sampler, x, &nozzle_flow);
            }
            mail_gas_mail(&nozzle_flow.gas_mail);
        }
    }
}

static double
calc_engine_torque_n_m(const struct engine_s* self)
{
    double torque_n_m = 0.0;
    for(size_t i = 0; i < self->size; i++)
    {
        struct node_s* node = &self->node[i];
        if(node->type == is_piston)
        {
            torque_n_m += calc_piston_gas_torque_n_m(&node->as.piston, &self->crankshaft);
            torque_n_m += calc_piston_inertia_torque_n_m(&node->as.piston, &self->crankshaft);
            torque_n_m += calc_piston_friction_torque_n_m(&node->as.piston, &self->crankshaft);
        }
    }
    torque_n_m += calc_starter_torque_n_m(&self->starter, &self->flywheel, &self->crankshaft);
    return torque_n_m;
}

static double
calc_engine_moment_of_inertia_kg_m2(const struct engine_s* self)
{
    double moment_of_inertia_kg_m2 = 0.0;
    for(size_t i = 0; i < self->size; i++)
    {
        struct node_s* node = &self->node[i];
        if(node->type == is_piston)
        {
            moment_of_inertia_kg_m2 += calc_piston_moment_of_inertia_kg_per_m2(&node->as.piston);
        }
    }
    moment_of_inertia_kg_m2 += calc_crankshaft_moment_of_inertia_kg_m2(&self->crankshaft);
    moment_of_inertia_kg_m2 += calc_flywheel_moment_of_inertia_kg_m2(&self->flywheel);
    return moment_of_inertia_kg_m2;
}

static void
move_engine(struct engine_s* self, struct sampler_s* sampler)
{
    double torque_n_m = calc_engine_torque_n_m(self);
    double moment_of_inertia_kg_m2 = calc_engine_moment_of_inertia_kg_m2(self);
    double angular_acceleration_r_per_s2 = torque_n_m / moment_of_inertia_kg_m2;
    accelerate_crankshaft(&self->crankshaft, angular_acceleration_r_per_s2);
    double theta_0_r = self->crankshaft.theta_r;
    turn_crankshaft(&self->crankshaft);
    double theta_1_r = self->crankshaft.theta_r;
    if(theta_0_r != theta_1_r)
    {
        double theta_x_r = fmod(theta_0_r, g_std_four_pi_r);
        double theta_y_r = fmod(theta_1_r, g_std_four_pi_r);
        if(theta_y_r < theta_x_r)
        {
            sampler->size = sampler->index;
            sampler->index = 0;
        }
        else
        {
            sampler->index += 1;
            sampler->index = min(sampler->index, g_sampler_max_samples - 1);
        }
    }
}

static void
compress_engine_pistons(struct engine_s* self)
{
    for(size_t i = 0; i < self->size; i++)
    {
        struct node_s* node = &self->node[i];
        if(node->type == is_piston)
        {
            compress_piston(&node->as.piston, &self->crankshaft);
        }
    }
}

static void
reset_engine(struct engine_s* self)
{
    rig_engine_pistons(self);
    normalize_engine(self);
    select_nodes(self->node, self->size, is_piston);
}

static void
run_engine(struct engine_s* self, struct engine_time_s* engine_time, struct sampler_s* sampler)
{
    for(size_t i = 0; i < g_engine_cycles_per_frame; i++)
    {
        sampler->channel_index = 0;
        uint64_t t0 = engine_time->get_ms();
        flow_engine(self, sampler);
        uint64_t t1 = engine_time->get_ms();
        uint64_t t2 = engine_time->get_ms();
        uint64_t t3 = engine_time->get_ms();
        move_engine(self, sampler);
        compress_engine_pistons(self);
        uint64_t t4 = engine_time->get_ms();
        engine_time->fluids_time_ms += t1 - t0;
        engine_time->thermo_time_ms += t2 - t1;
        engine_time->tbd_time_ms += t3 - t2;
        engine_time->kinematics_time_ms += t4 - t3;
    }
}
