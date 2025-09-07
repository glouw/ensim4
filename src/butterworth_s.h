static constexpr size_t g_butterworth_order = 4;

struct butterworth_biquad_s
{
    double x1;
    double x2;
    double y1;
    double y2;
    double a0;
    double a1;
    double a2;
    double b0;
    double b1;
    double b2;
};

struct butterworth_s
{
    size_t cycles;
    struct butterworth_biquad_s stages[g_butterworth_order];
};

static double
butterworth_biquad_lowpass_filter(struct butterworth_biquad_s* self, double value, double cutoff_frequency_hz, double theta_r)
{
    double wc = 2.0 * g_std_audio_sample_rate_hz * tan(g_std_pi_r * cutoff_frequency_hz / g_std_audio_sample_rate_hz);
    double s_real = cos(theta_r) * wc;
    double s_imag = sin(theta_r) * wc;
    double T = 1.0 / g_std_audio_sample_rate_hz;
    double c = 2.0 / T;
    double k1 = -2.0 * s_real / c;
    double k2 = (s_real*s_real + s_imag*s_imag) / (c*c);
    self->a0 = 1.0 + k1 + k2;
    self->b0 = k2 / self->a0;
    self->b1 = 2.0 * self->b0;
    self->b2 = self->b0;
    self->a1 = 2.0 * (k2 - 1.0) / self->a0;
    self->a2 = (1.0 - k1 + k2) / self->a0;
    double y = self->b0*value + self->b1*self->x1 + self->b2*self->x2 - self->a1*self->y1 - self->a2*self->y2;
    self->x2 = self->x1;
    self->x1 = value;
    self->y2 = self->y1;
    self->y1 = y;
    return y;
}

static double
butterworth_lowpass_filter(struct butterworth_s* self, double value, double cutoff_frequency_hz)
{
    if(self->cycles == 0)
    {
        struct butterworth_biquad_s stage = {
            .x1 = value,
            .x2 = value,
            .y1 = value,
            .y2 = value,
        };
        for(size_t i = 0; i < g_butterworth_order; i++)
        {
            self->stages[i] = stage;
        }
    }
    for(size_t k = 0; k < g_butterworth_order / 2; k++)
    {
        double theta_r = g_std_pi_r * (2.0 * (k + 1.0) + g_butterworth_order - 1.0) / (2.0 * g_butterworth_order);
        value = butterworth_biquad_lowpass_filter(&self->stages[k], value, cutoff_frequency_hz, theta_r);
    }
    self->cycles += 1;
    return value;
}
