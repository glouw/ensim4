struct lowpass_filter_s
{
    double x0;
    double y0;
};

static double
filter_lowpass(struct lowpass_filter_s* self, double cutoff_frequency_hz, double sample)
{
    double rc_constant = 1.0 / (2.0 * g_std_pi_r * cutoff_frequency_hz);
    double alpha = g_std_dt_s / (rc_constant + g_std_dt_s);
    double output = alpha * sample + (1.0 - alpha) * self->y0;
    self->x0 = sample;
    self->y0 = output;
    return output;
}
