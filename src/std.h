static constexpr size_t std_cacheline_bytes = 64;
static constexpr size_t std_audio_sample_rate_hz = 44100;
static constexpr double std_dt_s = 1.0 / std_audio_sample_rate_hz;
static constexpr double std_pi_r = 3.141592653589793;
static constexpr double std_four_pi_r = 4.0 * std_pi_r;

#define len(x) (sizeof(x) / sizeof(x[0]))
#define min(x, y) (((x) < (y)) ? (x) : (y))
#define max(x, y) (((x) > (y)) ? (x) : (y))
#define clear(x) memset(x, 0, sizeof(x));
