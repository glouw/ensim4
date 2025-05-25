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
sample_engine_channel(struct node_s* node, double placeholder_r)
{
    sample_engine_name(static_pressure_pa, calc_static_pressure_pa(&node->as.chamber));
    sample_engine_name(total_pressure_pa, calc_total_pressure_pa(&node->as.chamber));
    sample_engine_name(static_temperature_k, node->as.chamber.gas.static_temperature_k);
    sample_engine_name(volume_m3, node->as.chamber.volume_m3);
    sample_engine_name(placeholder_0, placeholder_r);
    sample_engine_name(placeholder_1, placeholder_r);
    sample_engine_name(placeholder_2, placeholder_r);
    sample_engine_name(placeholder_3, placeholder_r);
    sample_engine_name(placeholder_4, placeholder_r);
    sample_engine_name(placeholder_5, placeholder_r);
    sample_engine_name(placeholder_5, placeholder_r);
    sample_engine_name(placeholder_6, placeholder_r);
    sample_engine_name(placeholder_7, placeholder_r);
    sample_engine_name(placeholder_8, placeholder_r);
    sample_engine_name(placeholder_9, placeholder_r);
}

static void
sample_engine(struct engine_s* self)
{
    sample_channel_index = 0;
    for(size_t i = 0; i < self->size; i++)
    {
        struct node_s* node = &self->node[i];
        if(node->is_selected)
        {
            double placeholder_r = 1.0 - cos(self->crankshaft.theta_r + sample_channel_index);
            sample_engine_channel(node, placeholder_r);
            if(++sample_channel_index == sample_channels)
            {
                break;
            }
        }
    }
}

static void
flow_engine(struct engine_s* self, struct gas_mail_s gas_mails[])
{
    size_t mail_index = 0;
    for(size_t i = 0; i < self->size; i++)
    {
        struct node_s* x = &self->node[i];
        for(size_t next, j = 0; (next = x->next[j]); j++)
        {
            struct node_s* y = &self->node[next];
            struct nozzle_flow_s nozzle_flow = flow(&x->as.chamber, &y->as.chamber);
            gas_mails[mail_index++] = nozzle_flow.gas_mail;
        }
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
mail_engine(struct engine_s* self, struct gas_mail_s gas_mails[])
{
    for(size_t i = 0; i < self->edges; i++)
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
