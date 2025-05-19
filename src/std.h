static constexpr double std_audio_sample_rate_hz = 44100.0;
static constexpr double std_dt_s = 1.0 / std_audio_sample_rate_hz;
static constexpr double std_pi_r = 3.141592653589793;

static double
clamp(double value, double x, double y)
{
    return value < x ? x : value > y ? y : value;
}

#define len(x) (sizeof(x) / sizeof(x[0]))
