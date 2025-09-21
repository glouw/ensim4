struct lowpass_filter_s
{
    double last;
};

static double
filter_lowpass(struct lowpass_filter_s* self, double cutoff_frequency_hz, double sample)
{
    double rc_constant = 1.0 / (2.0 * g_std_pi_r * cutoff_frequency_hz);
    double alpha = g_std_dt_s / (rc_constant + g_std_dt_s);
    double output = alpha * sample + (1.0 - alpha) * self->last;
    self->last = output;
    return output;
}

struct lowpass_filter_3_s
{
    struct lowpass_filter_s a;
    struct lowpass_filter_s b;
    struct lowpass_filter_s c;
};

static double
filter_lowpass_3(struct lowpass_filter_3_s* self, double cutoff_frequency_hz, double sample)
{
    sample = filter_lowpass(&self->a, cutoff_frequency_hz, sample);
    sample = filter_lowpass(&self->b, cutoff_frequency_hz, sample);
    sample = filter_lowpass(&self->c, cutoff_frequency_hz, sample);
    return sample;
}
