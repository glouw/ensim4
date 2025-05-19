static constexpr size_t plot_channels = 8;
static constexpr size_t plot_samples = (size_t) audio_sample_rate_hz;

struct sample_s
{
    float static_pressure_pa;
};

struct sample_s plot_g[plot_channels][plot_samples];
