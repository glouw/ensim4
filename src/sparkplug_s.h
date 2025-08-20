struct sparkplug_s
{
    double engage_r;
    double on_r;
};

static bool
is_sparkplug_on(struct sparkplug_s* self, struct crankshaft_s* crankshaft)
{
    double current_theta_r = fmod(crankshaft->theta_r, g_std_four_pi_r);
    double normalized_engage_r = fmod(self->engage_r, g_std_four_pi_r);
    if(normalized_engage_r < 0.0)
    {
        normalized_engage_r += g_std_four_pi_r;
    }
    double end_spark_r = normalized_engage_r + self->on_r;
    if(end_spark_r > g_std_four_pi_r)
    {
        return (current_theta_r >= normalized_engage_r && current_theta_r < g_std_four_pi_r) ||
               (current_theta_r >= 0.0 && current_theta_r < (end_spark_r - g_std_four_pi_r));
    }
    else
    {
        return (current_theta_r >= normalized_engage_r && current_theta_r < end_spark_r);
    }
}
