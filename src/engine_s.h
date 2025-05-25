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
sample_engine_channel(struct node_s* node, double sampled_nozzle_area_m2, struct nozzle_flow_field_s nozzle_flow_field)
{
    if(sample_channel_index < sample_channels)
    {
        sample_engine_name(static_pressure_pa, calc_static_pressure_pa(&node->as.chamber));
        sample_engine_name(total_pressure_pa, calc_total_pressure_pa(&node->as.chamber));
        sample_engine_name(static_temperature_k, node->as.chamber.gas.static_temperature_k);
        sample_engine_name(volume_m3, node->as.chamber.volume_m3);
        sample_engine_name(nozzle_area_m2, sampled_nozzle_area_m2);
        sample_engine_name(nozzle_mach, nozzle_flow_field.mach);
        sample_engine_name(nozzle_velocity_m_per_s, nozzle_flow_field.velocity_m_per_s);
        sample_engine_name(nozzle_mass_flow_rate_kg_per_s, nozzle_flow_field.mass_flow_rate_kg_per_s);
        sample_engine_name(nozzle_speed_of_sound_m_per_s, nozzle_flow_field.speed_of_sound_m_per_s);
        sample_engine_name(placeholder_5, 0.0);
        sample_engine_name(placeholder_5, 0.0);
        sample_engine_name(placeholder_6, 0.0);
        sample_engine_name(placeholder_7, 0.0);
        sample_engine_name(placeholder_8, 0.0);
        sample_engine_name(placeholder_9, 0.0);
        sample_channel_index++;
    }
}

struct engine_flow_s
{
    struct node_s* x;
    struct node_s* y;
};

static int
compare_engine_flow(const void* x, const void* y)
{
    const struct engine_flow_s* a = x;
    const struct engine_flow_s* b = y;
    return (a->x > b->x) - (a->x < b->x);
}

static size_t
stage_engine_flow(struct engine_s* self, struct engine_flow_s engine_flow[])
{
    size_t flow_size = 0;
    for(size_t i = 0; i < self->size; i++)
    {
        struct node_s* x = &self->node[i];
        for(size_t next, j = 0; (next = x->next[j]); j++)
        {
            struct node_s* y = &self->node[next];
            engine_flow[flow_size++] = (struct engine_flow_s) {x, y};
        }
    }
    qsort(engine_flow, flow_size, sizeof(struct engine_flow_s), compare_engine_flow);
    return flow_size;
}

static size_t
flow_engine(struct engine_flow_s engine_flow[], size_t flow_size, struct gas_mail_s gas_mails[])
{
    sample_channel_index = 0;
    size_t mail_size = 0;
    struct node_s* last = nullptr;
    struct nozzle_flow_field_s flow_field = {};
    for(size_t i = 0; i < flow_size; i++)
    {
        struct node_s* x = engine_flow[i].x;
        struct node_s* y = engine_flow[i].y;
        struct nozzle_flow_s nozzle_flow = flow(&x->as.chamber, &y->as.chamber);
        if(nozzle_flow.is_success)
        {
            gas_mails[mail_size++] = nozzle_flow.gas_mail;
        }
        if(x == last)
        {
            flow_field = sum_nozzle_flow_fields(flow_field, nozzle_flow.flow_field);
            continue;
        }
        else
        {
            flow_field = nozzle_flow.flow_field;
        }
        last = x;
        if(x->is_selected)
        {
            sample_engine_channel(x, nozzle_flow.area_m2, flow_field);
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
