static constexpr size_t log_channels = 8;
static constexpr size_t log_samples = std_audio_sample_rate_hz;

typedef struct sample_s log_t[log_channels][log_samples];

static log_t log_log;
