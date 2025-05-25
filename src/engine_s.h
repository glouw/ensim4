static constexpr size_t engine_cycles_per_frame = 512;

struct engine_s
{
    struct node_s* node;
    size_t size;
    size_t edges;
    size_t bytes;
    struct crankshaft_s crankshaft;
};

#define set_engine(nodes) (struct engine_s) {          \
    .node = nodes,                                     \
    .size = len(nodes),                                \
    .edges = count_many_node_edges(nodes, len(nodes)), \
    .bytes = sizeof(nodes)                             \
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
sample_engine_name(enum sample_name_e sample_name, float sample)
{
    sample_sample[sample_channel_index][sample_name][sample_index] = sample;
}

static void
sample_engine_channel(
    struct node_s* node,
    double placeholder_r,
    double sampled_nozzle_area_m2,
    double sampled_nozzle_mach,
    double sampled_nozzle_velocity_m_per_s,
    double sampled_nozzle_mass_flow_rate_kg_per_s,
    double sampled_nozzle_speed_of_sound_m_per_s)
{
    if(sample_channel_index < sample_channels)
    {
        sample_engine_name(static_pressure_pa, calc_static_pressure_pa(&node->as.chamber));
        sample_engine_name(total_pressure_pa, calc_total_pressure_pa(&node->as.chamber));
        sample_engine_name(static_temperature_k, node->as.chamber.gas.static_temperature_k);
        sample_engine_name(volume_m3, node->as.chamber.volume_m3);
        sample_engine_name(nozzle_area_m2, sampled_nozzle_area_m2);
        sample_engine_name(nozzle_mach, sampled_nozzle_mach);
        sample_engine_name(nozzle_velocity_m_per_s, sampled_nozzle_velocity_m_per_s);
        sample_engine_name(nozzle_mass_flow_rate_kg_per_s, sampled_nozzle_mass_flow_rate_kg_per_s);
        sample_engine_name(nozzle_speed_of_sound_m_per_s, sampled_nozzle_speed_of_sound_m_per_s);
        sample_engine_name(placeholder_5, placeholder_r);
        sample_engine_name(placeholder_5, placeholder_r);
        sample_engine_name(placeholder_6, placeholder_r);
        sample_engine_name(placeholder_7, placeholder_r);
        sample_engine_name(placeholder_8, placeholder_r);
        sample_engine_name(placeholder_9, placeholder_r);
        sample_channel_index++;
    }
}

static size_t
flow_engine(struct engine_s* self, struct gas_mail_s gas_mails[])
{
    sample_channel_index = 0;
    size_t mail_size = 0;
    for(size_t i = 0; i < self->size; i++)
    {
        struct node_s* x = &self->node[i];
        double nozzle_area_m2 = 0.0;
        double nozzle_mach = 0.0;
        double nozzle_velocity_m_per_s = 0.0;
        double nozzle_mass_flow_rate_kg_per_s = 0.0;
        double nozzle_speed_of_sound_m_per_s = 0.0;
        for(size_t next, j = 0; (next = x->next[j]); j++)
        {
            struct node_s* y = &self->node[next];
            struct nozzle_flow_s nozzle_flow = flow(&x->as.chamber, &y->as.chamber);
            if(nozzle_flow.is_success)
            {
                gas_mails[mail_size++] = nozzle_flow.gas_mail;
            }
            nozzle_area_m2 += nozzle_flow.area_m2;
            nozzle_mach += nozzle_flow.mach;
            nozzle_velocity_m_per_s += nozzle_flow.velocity_m_per_s;
            nozzle_mass_flow_rate_kg_per_s += nozzle_flow.mass_flow_rate_kg_per_s;
            nozzle_speed_of_sound_m_per_s += nozzle_flow.speed_of_sound_m_per_s;
        }
        if(x->is_selected)
        {
            double placeholder_r = 1.0 - cos(self->crankshaft.theta_r + sample_channel_index);
            sample_engine_channel(
                x,
                placeholder_r,
                nozzle_area_m2,
                nozzle_mach,
                nozzle_velocity_m_per_s,
                nozzle_mass_flow_rate_kg_per_s,
                nozzle_speed_of_sound_m_per_s);
        }
    }
    return mail_size;
}

static void
move_engine(struct engine_s* self)
{
    double theta_0_r = self->crankshaft.theta_r;
    turn_crankshaft(&self->crankshaft);
    double theta_1_r = self->crankshaft.theta_r;
    if(theta_0_r != theta_1_r)
    {
        double theta_x_r = fmod(theta_0_r, std_four_pi_r);
        double theta_y_r = fmod(theta_1_r, std_four_pi_r);
        if(theta_y_r < theta_x_r)
        {
            sample_size = sample_index;
            sample_index = 0;
        }
        else
        {
            sample_index += 1;
            sample_index = min(sample_index, sample_samples - 1);
        }
    }
}

static void
mail_engine(struct engine_s* self, struct gas_mail_s gas_mails[], size_t mail_size)
{
    for(size_t i = 0; i < mail_size; i++)
    {
        mail_gas_mail(&gas_mails[i]);
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
