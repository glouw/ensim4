static constexpr size_t g_sample_channels = 16;
static constexpr size_t g_sample_samples = 8192;
static constexpr double g_sample_minimum_angular_velocity_r_per_s = g_std_four_pi_r * g_std_audio_sample_rate_hz / g_sample_samples;

#define SAMPLES                                \
    X(g_sample_static_pressure_pa)             \
    X(g_sample_total_pressure_pa)              \
    X(g_sample_static_temperature_k)           \
    X(g_sample_volume_m3)                      \
    X(g_sample_nozzle_area_m2)                 \
    X(g_sample_nozzle_mach)                    \
    X(g_sample_nozzle_velocity_m_per_s)        \
    X(g_sample_nozzle_mass_flow_rate_kg_per_s) \
    X(g_sample_nozzle_speed_of_sound_m_per_s)  \

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

static float g_sample_sample[g_sample_channels][g_sample_name_e_size][g_sample_samples];
static size_t g_sample_index = 0;
static size_t g_sample_channel_index = 0;
static size_t g_sample_size = 0;

static void
clear_samples()
{
    clear(g_sample_sample);
}

static const char*
skip_sample_namespace(const char* string)
{
    char* g_ = strchr(string, '_') + 1;
    return strchr(g_, '_') + 1;
}
