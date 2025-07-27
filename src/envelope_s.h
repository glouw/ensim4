struct envelope_s
{
    double max_gain;
    double limiter;
    double gain;
};

static void
process_envelope(struct envelope_s* self, double signal)
{
    signal = fabs(signal);
    if(signal > self->limiter)
    {
        self->gain = self->max_gain;
    }
    else
    {
        self->gain = self->max_gain * (signal / self->limiter);
    }
}
