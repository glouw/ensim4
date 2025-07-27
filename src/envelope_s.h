struct envelope_s
{
    double max_gain;
    double limiter;
    double gain;
    struct moving_average_filter_s moving_average_filter;
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
    self->gain = push_moving_average_filter(&self->moving_average_filter, self->gain);
}
