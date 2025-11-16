constexpr size_t g_std_audio_sample_rate_hz = 48000;
constexpr size_t g_std_monitor_refresh_rate = ENSIM4_MONITOR_REFRESH_RATE_HZ;
constexpr double g_std_dt_s = 1.0 / g_std_audio_sample_rate_hz;
constexpr double g_std_pi_r = 3.141592653589793;
constexpr double g_std_four_pi_r = 4.0 * g_std_pi_r;

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
    return g_std_pi_r * pow(diameter_m / 2.0, 2.0);
}

static double
calc_cylinder_volume_m3(double diameter_m, double depth_m)
{
    return calc_circle_area_m2(diameter_m) * depth_m;
}

/*       w1 * x1 + w2 * x2
 * mix = -----------------
 *            w1 + w
 */

static double
calc_mix(double value1, double weight1, double value2, double weight2)
{
    return (value1 * weight1 + value2 * weight2) / (weight1 + weight2);
}
