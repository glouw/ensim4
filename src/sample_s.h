constexpr size_t max_plot_channels = 8;
constexpr size_t max_plot_samples = (size_t) audio_sample_rate_hz;

struct sample_s
{
    float static_pressure_pa;
};

struct sample_s samples[max_plot_channels][max_plot_samples];
