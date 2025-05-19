#define SAMPLES             \
    X(static_pressure_pa)   \
    X(dynamic_pressure_pa)  \
    X(static_temperature_k) \

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
