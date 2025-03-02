static double
clamp(double value, double x, double y)
{
    return value < x ? x : value > y ? y : value;
}

constexpr size_t max_edges_per_node = 16;
constexpr size_t nodes_per_engine = 16;
constexpr size_t cache_line_bytes = 128;
constexpr double audio_sample_rate_hz = 44100.0;
constexpr double dt_s = 1.0 / audio_sample_rate_hz;
