static constexpr size_t sample_channels = 4;
static constexpr size_t sample_samples = 8192;
static constexpr double sample_minimum_angular_velocity_r_per_s = 4.0 * std_pi_r * std_audio_sample_rate_hz / sample_samples;

#define SAMPLES             \
    X(static_pressure_pa)   \
    X(dynamic_pressure_pa)  \
    X(static_temperature_k) \
    X(volume_m3)            \
    X(placeholder_0)        \
    X(placeholder_1)        \
    X(placeholder_2)        \
    X(placeholder_3)        \
    X(placeholder_4)        \
    X(placeholder_5)        \
    X(placeholder_6)        \
    X(placeholder_7)        \
    X(placeholder_8)        \
    X(placeholder_9)        \

enum sample_e
{
    #define X(N) N,
    SAMPLES
    #undef X
    sample_e_size
};

static const char* sample_name[] = {
    #define X(N) #N,
    SAMPLES
    #undef X
};

static float sample_sample[sample_channels][sample_e_size][sample_samples];
static size_t sample_size = 0;
static size_t sample_index = 0;

#undef SAMPLES
