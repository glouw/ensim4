static constexpr size_t sample_channels = 8;
static constexpr size_t sample_samples = 8192;
static constexpr double sample_minimum_angular_velocity_r_per_s = std_four_pi_r * std_audio_sample_rate_hz / sample_samples;

#define SAMPLES                       \
    X(static_pressure_pa)             \
    X(total_pressure_pa)              \
    X(static_temperature_k)           \
    X(volume_m3)                      \
    X(nozzle_area_m2)                 \
    X(nozzle_mach)                    \
    X(nozzle_velocity_m_per_s)        \
    X(nozzle_mass_flow_rate_kg_per_s) \
    X(nozzle_speed_of_sound_m_per_s ) \
    X(placeholder_5)                  \
    X(placeholder_6)                  \
    X(placeholder_7)                  \
    X(placeholder_8)                  \
    X(placeholder_9)                  \

enum sample_name_e
{
    #define X(N) N,
    SAMPLES
    #undef X
    sample_name_e_size
};

static const char* sample_name_string[] = {
    #define X(N) #N,
    SAMPLES
    #undef X
};

#undef SAMPLES

typedef float sample_t[sample_channels][sample_name_e_size][sample_samples];

static sample_t sample_sample = {};
static size_t sample_index = 0;
static size_t sample_channel_index = 0;
static size_t sample_size = 0;

static void
clear_samples()
{
    clear(sample_sample);
}
