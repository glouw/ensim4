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
sample_engine(struct engine_s* self)
{
    size_t channel = 0;
    for(size_t i = 0; i < self->size; i++)
    {
        struct node_s* node = &self->node[i];
        double temp_r = 5.0 * (1.0 - cos(self->theta_r + channel));
        sample_sample[channel][static_pressure_pa][sample_index] = temp_r;
        sample_sample[channel][dynamic_pressure_pa][sample_index] = temp_r;
        sample_sample[channel][static_temperature_k][sample_index] = temp_r;
        sample_sample[channel][volume_m3][sample_index] = temp_r;
        sample_sample[channel][placeholder_0][sample_index] = temp_r;
        sample_sample[channel][placeholder_1][sample_index] = temp_r;
        sample_sample[channel][placeholder_2][sample_index] = temp_r;
        sample_sample[channel][placeholder_3][sample_index] = temp_r;
        sample_sample[channel][placeholder_4][sample_index] = temp_r;
        sample_sample[channel][placeholder_5][sample_index] = temp_r;
        sample_sample[channel][placeholder_5][sample_index] = temp_r;
        sample_sample[channel][placeholder_6][sample_index] = temp_r;
        sample_sample[channel][placeholder_7][sample_index] = temp_r;
        sample_sample[channel][placeholder_8][sample_index] = temp_r;
        sample_sample[channel][placeholder_9][sample_index] = temp_r;
        if(node->is_selected)
        {
            channel++;
        }
        if(channel == sample_channels)
        {
            break;
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
