#define SAMPLES             \
    X(static_pressure_pa)   \
    X(dynamic_pressure_pa)  \
    X(static_temperature_k) \
    X(sample_0) \
    X(sample_1) \
    X(sample_2) \
    X(sample_3) \
    X(sample_4) \
    X(sample_5) \
    X(sample_6) \

struct sample_s
{
    #define X(sample) float sample;
    SAMPLES
    #undef X
};

static const char* sample_name[] = {
    #define X(sample) #sample,
    SAMPLES
    #undef X
};

#undef SAMPLES
