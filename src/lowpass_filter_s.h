struct lowpass_filter_s
{
    double prev_input;
    double prev_output;
};

static double
filter_lowpass(struct lowpass_filter_s* self, double cutoff_frequency_hz, double sample)
{
    double rc_constant = 1.0 / (2.0 * g_std_pi_r * cutoff_frequency_hz);
    double alpha = g_std_dt_s / (rc_constant + g_std_dt_s);
    double output = alpha * sample + (1.0 - alpha) * self->prev_output;
    self->prev_input = sample;
    self->prev_output = output;
    return output;
}
