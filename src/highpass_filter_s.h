struct highpass_filter_s
{
    double prev_input;
    double prev_output;
};

static double
filter_highpass(struct highpass_filter_s* self, double cutoff_frequency_hz, double sample)
{
    double rc_constant = 1.0 / (2.0 * g_std_pi_r * cutoff_frequency_hz);
    double alpha = rc_constant / (rc_constant + g_std_dt_s);
    double output = alpha * (self->prev_output + sample - self->prev_input);
    self->prev_input = sample;
    self->prev_output = output;
    return output;
}
