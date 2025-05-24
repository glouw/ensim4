static constexpr size_t engine_cycles_per_frame = 512;

struct engine_s
{
    struct node_s* node;
    size_t size;
    size_t bytes;
    double angular_velocity_r_per_s;
    double theta_r;
};

#define set_engine(nodes) (struct engine_s) { .node = nodes, .size = len(nodes), .bytes = sizeof(nodes) }

static void
normalize_engine(struct engine_s* self)
{
    for(size_t i = 0; i < self->size; i++)
    {
        normalize_node(&self->node[i]);
    }
}

static void
sample_engine_channel_name(enum sample_name_e sample_name, float sample)
{
    sample_sample[sample_channel_index][sample_name][sample_index] = sample;
}

static void
sample_engine_channel(struct engine_s* self)
{
    double temp_r = 1.0 - cos(self->theta_r + sample_channel_index);
    sample_engine_channel_name(static_pressure_pa, temp_r);
    sample_engine_channel_name(dynamic_pressure_pa, temp_r);
    sample_engine_channel_name(static_temperature_k, temp_r);
    sample_engine_channel_name(volume_m3, temp_r);
    sample_engine_channel_name(placeholder_0, temp_r);
    sample_engine_channel_name(placeholder_1, temp_r);
    sample_engine_channel_name(placeholder_2, temp_r);
    sample_engine_channel_name(placeholder_3, temp_r);
    sample_engine_channel_name(placeholder_4, temp_r);
    sample_engine_channel_name(placeholder_5, temp_r);
    sample_engine_channel_name(placeholder_5, temp_r);
    sample_engine_channel_name(placeholder_6, temp_r);
    sample_engine_channel_name(placeholder_7, temp_r);
    sample_engine_channel_name(placeholder_8, temp_r);
    sample_engine_channel_name(placeholder_9, temp_r);
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
            sample_engine_channel(self);
            if(++sample_channel_index == sample_channels)
            {
                break;
            }
        }
    }
}

static void
flow_engine(struct engine_s* self)
{
    for(size_t i = 0; i < self->size; i++)
    {
        struct node_s* x = &self->node[i];
        for(size_t next, j = 0; (next = x->next[j]); j++)
        {
            struct node_s* y = &self->node[next];
            struct nozzle_flow_s nozzle_flow = flow(&x->as.chamber, &y->as.chamber);
            // sum these three from nozzle_flow_s if a parent has more than 1 child:
            //   mach
            //   velocity_m_per_s
            //   mass_flow_rate_kg_per_s
        }
    }
}

static void
move_engine(struct engine_s* self)
{
    double theta_0_r = self->theta_r;
    self->theta_r += self->angular_velocity_r_per_s * std_dt_s;
    double theta_1_r = self->theta_r;
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
run_engine(struct engine_s* self)
{
    for(size_t i = 0; i < engine_cycles_per_frame; i++)
    {
        flow_engine(self);
        move_engine(self);
        sample_engine(self);
    }
}
