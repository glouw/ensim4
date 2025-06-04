static constexpr size_t std_cacheline_bytes = 64;
static constexpr size_t std_audio_sample_rate_hz = 44100;
static constexpr double std_dt_s = 1.0 / std_audio_sample_rate_hz;
static constexpr double std_pi_r = 3.141592653589793;
static constexpr double std_four_pi_r = 4.0 * std_pi_r;

#define len(x) (sizeof(x) / sizeof(x[0]))
#define clear(x) memset(x, 0, sizeof(x));
#define swap(x, y) { auto copy = (x); (x) = (y); (y) = copy; }

static double
min(double x, double y)
{
    return x < y ? x : y;
}

static double
max(double x, double y)
{
    return x > y ? x : y;
}

static double
clamp(double value, double lower, double upper)
{
    return value < lower ? lower : value > upper ? upper : value;
}

static double
calc_circle_area_m2(double diameter_m)
{
    return std_pi_r * pow(diameter_m / 2.0, 2.0);
}

static double
calc_cylinder_volume_m3(double diameter_m, double depth_m)
{
    return calc_circle_area_m2(diameter_m) * depth_m;
}
