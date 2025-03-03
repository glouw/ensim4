constexpr size_t cache_line_bytes = 64;
constexpr double audio_sample_rate_hz = 44100.0;
constexpr double dt_s = 1.0 / audio_sample_rate_hz;

static double
clamp(double value, double x, double y)
{
    return value < x ? x : value > y ? y : value;
}
