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
sample_engine_channel(struct node_s* node, struct nozzle_flow_s* nozzle_flow)
{
    if(sample_channel_index < sample_channels)
    {
        sample_engine_name(sample_static_pressure_pa, calc_static_pressure_pa(&node->as.chamber));
        sample_engine_name(sample_total_pressure_pa, calc_total_pressure_pa(&node->as.chamber));
        sample_engine_name(sample_static_temperature_k, node->as.chamber.gas.static_temperature_k);
        sample_engine_name(sample_volume_m3, node->as.chamber.volume_m3);
        sample_engine_name(sample_nozzle_area_m2, nozzle_flow->area_m2);
        sample_engine_name(sample_nozzle_mach, nozzle_flow->flow_field.mach);
        sample_engine_name(sample_nozzle_velocity_m_per_s, nozzle_flow->flow_field.velocity_m_per_s);
        sample_engine_name(sample_nozzle_mass_flow_rate_kg_per_s, nozzle_flow->flow_field.mass_flow_rate_kg_per_s);
        sample_engine_name(sample_nozzle_speed_of_sound_m_per_s, nozzle_flow->flow_field.speed_of_sound_m_per_s);
        sample_engine_name(sample_placeholder_5, 0.0);
        sample_engine_name(sample_placeholder_5, 0.0);
        sample_engine_name(sample_placeholder_6, 0.0);
        sample_engine_name(sample_placeholder_7, 0.0);
        sample_engine_name(sample_placeholder_8, 0.0);
        sample_engine_name(sample_placeholder_9, 0.0);
        sample_channel_index++;
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

struct engine_flow_s
{
    struct node_s* x;
    struct node_s* y;
};

static void
stage_engine_flow(
    struct engine_s* self,
    struct engine_flow_s engine_flows[])
{
    size_t k = 0;
    for(size_t i = 0; i < self->size; i++)
    {
        struct node_s* x = &self->node[i];
        for(size_t next, j = 0; (next = x->next[j]); j++)
        {
            struct node_s* y = &self->node[next];
            engine_flows[k++] = (struct engine_flow_s) {x, y};
        }
    }
}

static void
flow_engine(
    struct engine_s* self,
    struct engine_flow_s engine_flows[],
    struct nozzle_flow_s nozzle_flows[])
{
    for(size_t i = 0; i < self->edges; i++)
    {
        struct node_s* x = engine_flows[i].x;
        struct node_s* y = engine_flows[i].y;
        nozzle_flows[i] = flow(&x->as.chamber, &y->as.chamber);
    }
}

static void
sample_engine(
    struct engine_s* self,
    struct engine_flow_s engine_flows[],
    struct nozzle_flow_s nozzle_flows[])
{
    sample_channel_index = 0;
    for(size_t i = 0; i < self->edges; i++)
    {
        struct node_s* x = engine_flows[i].x;
        if(x->is_selected)
        {
            sample_engine_channel(x, &nozzle_flows[i]);
        }
    }
}

static void
mail_engine(
    struct engine_s* self,
    struct nozzle_flow_s nozzle_flows[])
{
    for(size_t i = 0; i < self->edges; i++)
    {
        mail_gas_mail(&nozzle_flows[i].gas_mail);
    }
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
