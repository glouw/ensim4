static constexpr size_t g_sampler_max_channels = 16;
static constexpr size_t g_sampler_max_samples = 8192;
static constexpr double g_sampler_min_angular_velocity_r_per_s = g_std_four_pi_r * g_std_audio_sample_rate_hz / g_sampler_max_samples;

#define SAMPLES                                  \
    X(g_sample_static_pressure_pa)               \
    X(g_sample_total_pressure_pa)                \
    X(g_sample_static_temperature_k)             \
    X(g_sample_volume_m3)                        \
    X(g_sample_nozzle_area_m2)                   \
    X(g_sample_nozzle_mach)                      \
    X(g_sample_nozzle_velocity_m_per_s)          \
    X(g_sample_nozzle_mass_flow_rate_kg_per_s)   \
    X(g_sample_nozzle_speed_of_sound_m_per_s)    \
    X(g_sample_starter_angular_velocity_r_per_s) \
    X(g_sample_starter_gear_audio_sample)        \

enum sample_name_e
{
#define X(N) N,
    SAMPLES
#undef X
    g_sample_name_e_size
};

static const char* g_sample_name_string[] = {
#define X(N) #N,
    SAMPLES
#undef X
};

#undef SAMPLES

struct sampler_s
{
    float value[g_sampler_max_channels][g_sample_name_e_size][g_sampler_max_samples];
    size_t index;
    size_t channel_index;
    size_t size;
};

static void
sample_value(struct sampler_s* self, enum sample_name_e sample_name, float sample)
{
    self->value[self->channel_index][sample_name][self->index] = sample;
}

static void
sample_channel(struct sampler_s* self, struct node_s* node, struct nozzle_flow_s* nozzle_flow)
{
    if(self->channel_index< g_sampler_max_channels)
    {
        sample_value(self, g_sample_static_pressure_pa, calc_static_pressure_pa(&node->as.chamber));
        sample_value(self, g_sample_total_pressure_pa, calc_total_pressure_pa(&node->as.chamber));
        sample_value(self, g_sample_static_temperature_k, node->as.chamber.gas.static_temperature_k);
        sample_value(self, g_sample_volume_m3, node->as.chamber.volume_m3);
        sample_value(self, g_sample_nozzle_area_m2, nozzle_flow->area_m2);
        sample_value(self, g_sample_nozzle_mach, nozzle_flow->flow_field.mach);
        sample_value(self, g_sample_nozzle_velocity_m_per_s, nozzle_flow->flow_field.velocity_m_per_s);
        sample_value(self, g_sample_nozzle_mass_flow_rate_kg_per_s, nozzle_flow->flow_field.mass_flow_rate_kg_per_s);
        sample_value(self, g_sample_nozzle_speed_of_sound_m_per_s, nozzle_flow->flow_field.speed_of_sound_m_per_s);
        self->channel_index++;
    }
}

static void
sample_misc(struct sampler_s* self, struct starter_s* starter, struct flywheel_s* flywheel, struct crankshaft_s* crankshaft)
{
    sample_value(self, g_sample_starter_angular_velocity_r_per_s, calc_starter_angular_velocity_r_per_s(starter, flywheel, crankshaft));
    sample_value(self, g_sample_starter_gear_audio_sample, calc_starter_gear_audio_sample(starter, flywheel, crankshaft));
}

static void
clear_sampler(struct sampler_s* self)
{
    clear(self->value);
}

static void
reset_sampler_channel(struct sampler_s* self)
{
    self->channel_index = 0;
}

static const char*
skip_sample_namespace(const char* string)
{
    return strchr(strchr(string, '_') + 1, '_') + 1;
}
