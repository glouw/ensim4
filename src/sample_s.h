static constexpr size_t sample_max_channels = 8;
static constexpr size_t sample_max_samples = (size_t) std_audio_sample_rate_hz;

struct sample_s
{
    float static_pressure_pa;
};

struct sample_s sample_log[sample_max_channels][sample_max_samples];
