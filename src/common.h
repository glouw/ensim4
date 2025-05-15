constexpr double audio_sample_rate_hz = 44100.0;
constexpr double dt_s = 1.0 / audio_sample_rate_hz;
constexpr double pi_rad = 3.141592653589793;

static double
clamp(double value, double x, double y)
{
    return value < x ? x : value > y ? y : value;
}
