static constexpr double g_engine_stroke_ratio = 1.0 / 4.0;
static constexpr double g_engine_intake_stroke_theta_r = 0.0 * g_engine_stroke_ratio * g_std_four_pi_r;
static constexpr double g_engine_compression_stroke_theta_r = 1.0 * g_engine_stroke_ratio * g_std_four_pi_r;
static constexpr double g_engine_power_stroke_theta_r = 2.0 * g_engine_stroke_ratio * g_std_four_pi_r;
static constexpr double g_engine_exhaust_stroke_theta_r = 3.0 * g_engine_stroke_ratio * g_std_four_pi_r;

struct engine_s
{
    const char* name;
    struct node_s* node;
    size_t size;
    struct crankshaft_s crankshaft;
    struct flywheel_s flywheel;
    struct starter_s starter;
    struct limiter_s limiter;
    double throttle_open_ratio;
    double no_throttle;
    double low_throttle;
    double mid_throttle;
    double high_throttle;
    double radial_spacing;
    double volume;
    bool use_cfd;
    bool use_convolution;
    bool can_ignite;
    bool use_plot_filter;
};

struct engine_time_s
{
    double fluids_time_ms;
    double kinematics_time_ms;
    double thermo_time_ms;
    double synth_time_ms;
    double wave_time_ms;
    double (*get_ticks_ms)();
};

#define engine_is(engine) .node = engine, .size = len(engine)

static void
analyze_engine(const struct engine_s* self)
{
    for(size_t i = 0; i < self->size; i++)
    {
        struct node_s* node = &self->node[i];
        if(node->type == g_is_eplenum)
        {
            if(count_node_edges(node) != 1)
            {
                fprintf(stderr, "error: eplenum[%lu] requires exactly one next[] edge\n", i);
                exit(1);
            }
        }
        if(node->type == g_is_injector)
        {
            if(count_node_edges(node) != 1)
            {
                fprintf(stderr, "error: injector[%lu] requires exactly one next[] edge\n", i);
                exit(1);
            }
            struct node_s* next = &self->node[node->next[0]];
            if(next->type != g_is_piston)
            {
                fprintf(stderr, "error: injector[%lu] must connect directly to a piston\n", i);
                exit(1);
            }
        }
    }
}

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
        if(node->type == g_is_piston)
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
                sample_channel(sampler, x, &nozzle_flow, &self->crankshaft);
            }
            if(is_reservoir(x))
            {
                nozzle_flow.gas_mail.is_from_reservoir = true;
            }
            if(nozzle_flow.is_success)
            {
                mail_gas_mail(&nozzle_flow.gas_mail);
            }
            if(x->type == g_is_eplenum)
            {
                struct eplenum_s* eplenum = &x->as.eplenum;
                size_t wave_index = eplenum->wave_index;
                struct wave_prim_s prim = {
                    .r = nozzle_flow.flow_field.static_density_kg_per_m3,
                    .u = nozzle_flow.flow_field.velocity_m_per_s,
                    .p = nozzle_flow.flow_field.static_pressure_pa,
                };
                stage_wave(wave_index, prim);
            }
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
        if(node->type == g_is_piston)
        {
            torque_n_m += calc_piston_gas_torque_n_m(&node->as.piston, &self->crankshaft);
            torque_n_m += calc_piston_inertia_torque_n_m(&node->as.piston, &self->crankshaft);
            torque_n_m += calc_piston_friction_torque_n_m(&node->as.piston, &self->crankshaft);
        }
    }
    torque_n_m += calc_starter_torque_on_flywheel_n_m(&self->starter, &self->flywheel, &self->crankshaft);
    return torque_n_m;
}

static double
calc_engine_moment_of_inertia_kg_m2(const struct engine_s* self)
{
    double moment_of_inertia_kg_m2 = 0.0;
    for(size_t i = 0; i < self->size; i++)
    {
        struct node_s* node = &self->node[i];
        if(node->type == g_is_piston)
        {
            moment_of_inertia_kg_m2 += calc_piston_moment_of_inertia_kg_per_m2(&node->as.piston);
        }
    }
    moment_of_inertia_kg_m2 += calc_crankshaft_moment_of_inertia_kg_m2(&self->crankshaft);
    moment_of_inertia_kg_m2 += calc_flywheel_moment_of_inertia_kg_m2(&self->flywheel);
    return moment_of_inertia_kg_m2;
}

static void
crank_engine(struct engine_s* self, struct sampler_s* sampler)
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
    maybe_limit_engine(&self->limiter, &self->crankshaft, &self->can_ignite);
}

static void
combust_engine_piston_chambers(struct engine_s* self)
{
    for(size_t i = 0; i < self->size; i++)
    {
        struct node_s* node = &self->node[i];
        if(node->type == g_is_piston)
        {
            struct piston_s* piston = &node->as.piston;
            if(calc_sparkplug_voltage_v(&piston->sparkplug, &self->crankshaft) > 0.0)
            {
                combust_c8h18(&piston->chamber, 1.0);
            }
        }
    }
}

static void
compress_engine_pistons(struct engine_s* self)
{
    for(size_t i = 0; i < self->size; i++)
    {
        struct node_s* node = &self->node[i];
        if(node->type == g_is_piston)
        {
            compress_piston(&node->as.piston, &self->crankshaft);
        }
    }
}

static void
update_engine_nozzle_open_ratios(struct engine_s* self)
{
    for(size_t i = 0; i < self->size; i++)
    {
        struct node_s* node = &self->node[i];
        switch(node->type)
        {
            case g_is_piston:
            {
                struct piston_s* piston = &node->as.piston;
                piston->chamber.nozzle_open_ratio = calc_valve_nozzle_open_ratio(&piston->valve, &self->crankshaft);
                break;
            }
            case g_is_irunner:
            {
                struct irunner_s* irunner = &node->as.irunner;
                irunner->chamber.nozzle_open_ratio = calc_valve_nozzle_open_ratio(&irunner->valve, &self->crankshaft);
                break;
            }
            case g_is_injector:
            {
                struct injector_s* injector = &node->as.injector;
                struct chamber_s* chamber = &self->node[injector->nozzle_index].as.chamber;
                if(chamber->nozzle_open_ratio > 0.0)
                {
                    struct piston_s* piston = &self->node[node->next[0]].as.piston;
                    injector->chamber.nozzle_open_ratio =
                        calc_mol_air_fuel_ratio(&piston->chamber.gas) > g_gas_ideal_mol_air_fuel_ratio
                        ? 1.0
                        : 0.0;
                }
                else
                {
                    injector->chamber.nozzle_open_ratio = 0.0;
                }
                break;
            }
            case g_is_throttle:
            {
                struct throttle_s* throttle = &node->as.throttle;
                throttle->chamber.nozzle_open_ratio = self->throttle_open_ratio;
                break;
            }
            default:
            {
                struct chamber_s* chamber = &node->as.chamber;
                chamber->nozzle_open_ratio = 1.0;
                break;
            }
        }
    }
}

static void
enable_engine_cfd(struct engine_s* self, bool use_cfd)
{
    self->use_cfd = use_cfd;
    for(size_t i = 0; i < self->size; i++)
    {
        struct node_s* node = &self->node[i];
        if(node->type == g_is_eplenum)
        {
            node->as.eplenum.use_cfd = use_cfd;
        }
    }
}

static void
reset_engine(struct engine_s* self)
{
    analyze_engine(self);
    enable_engine_cfd(self, true);
    self->use_convolution = true;
    self->starter.is_on = false;
    self->throttle_open_ratio = 0.01;
    reset_all_waves();
    rig_engine_pistons(self);
    normalize_engine(self);
    select_nodes(self->node, self->size, g_is_piston);
}

static void
flip_engine_waves(const struct engine_s* self)
{
    for(size_t i = 0; i < self->size; i++)
    {
        struct node_s* node = &self->node[i];
        if(node->type == g_is_eplenum)
        {
            flip_wave(node->as.eplenum.wave_index);
        }
    }
}

static void
launch_engine_waves(const struct engine_s* self)
{
    for(size_t i = 0; i < self->size; i++)
    {
        struct node_s* node = &self->node[i];
        if(node->type == g_is_eplenum)
        {
            launch_eplenum_wave_thread(&node->as.eplenum);
        }
    }
}

static void
wait_for_engine_waves(const struct engine_s* self)
{
    for(size_t i = 0; i < self->size; i++)
    {
        struct node_s* node = &self->node[i];
        if(node->type == g_is_eplenum)
        {
            wait_for_eplenum_wave_thread(&node->as.eplenum);
        }
    }
}

static void
sum_engine_waves(const struct engine_s* self)
{
    clear_wave_buffer();
    for(size_t i = 0; i < self->size; i++)
    {
        struct node_s* node = &self->node[i];
        if(node->type == g_is_eplenum)
        {
            add_to_wave_buffer(node->as.eplenum.wave_index);
        }
    }
}

static void
push_engine_wave_buffer_to_synth(struct engine_s* self, struct synth_s* synth, sampler_synth_t sampler_synth)
{
    sum_engine_waves(self);
    for(size_t i = 0; i < g_synth_buffer_size; i++)
    {
        sampler_synth[i] = push_synth(synth, &self->crankshaft, g_wave_buffer_pa[i], self->use_convolution, self->volume);
    }
}

static void
step_engine(
    struct engine_s* self,
    struct engine_time_s* engine_time,
    struct sampler_s* sampler)
{
    reset_sampler_channel(sampler);
    double t0 = engine_time->get_ticks_ms();
    flow_engine(self, sampler);
    double t1 = engine_time->get_ticks_ms();
    crank_engine(self, sampler);
    compress_engine_pistons(self);
    update_engine_nozzle_open_ratios(self);
    double starter_angular_velocity_r_per_s = calc_starter_angular_velocity_r_per_s(&self->starter, &self->flywheel, &self->crankshaft);
    sample_starter(sampler, starter_angular_velocity_r_per_s);
    double t2 = engine_time->get_ticks_ms();
    if(self->can_ignite)
    {
        combust_engine_piston_chambers(self);
    }
    double t3 = engine_time->get_ticks_ms();
    engine_time->fluids_time_ms += t1 - t0;
    engine_time->kinematics_time_ms += t2 - t1;
    engine_time->thermo_time_ms += t3 - t2;
}

static void
run_engine_with_waves(
    struct engine_s* self,
    struct engine_time_s* engine_time,
    struct sampler_s* sampler,
    struct synth_s* synth,
    size_t audio_buffer_size,
    sampler_synth_t sampler_synth)
{
    if(audio_buffer_size < g_synth_buffer_max_size)
    {
        flip_engine_waves(self);
        launch_engine_waves(self);
        for(size_t i = 0; i < g_synth_buffer_size; i++)
        {
            step_engine(self, engine_time, sampler);
        }
        wait_for_engine_waves(self);
        double t1 = engine_time->get_ticks_ms();
        push_engine_wave_buffer_to_synth(self, synth, sampler_synth);
        double t2 = engine_time->get_ticks_ms();
        engine_time->synth_time_ms = t2 - t1;
    }
}

static void
run_engine(
    struct engine_s* self,
    struct engine_time_s* engine_time,
    struct sampler_s* sampler,
    struct synth_s* synth,
    size_t audio_buffer_size,
    sampler_synth_t sampler_synth)
{
    double t0 = engine_time->get_ticks_ms();
    run_engine_with_waves(self, engine_time, sampler, synth, audio_buffer_size, sampler_synth);
    double t3 = engine_time->get_ticks_ms();
    engine_time->wave_time_ms += t3 - t0;
}
