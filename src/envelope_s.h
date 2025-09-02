struct envelope_s
{
    double max_value;
    double limiter;
    double value;
    struct moving_average_filter_s moving_average_filter;
};

static void
process_linear_envelope(struct envelope_s* self, double signal)
{
    signal = fabs(signal);
    if(signal > self->limiter)
    {
        self->value = self->value;
    }
    else
    {
        self->value = self->max_value * (signal / self->limiter);
    }
    self->value = push_moving_average_filter(&self->moving_average_filter, self->value);
}
