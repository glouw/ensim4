struct dc_filter_s
{
    double prev_input;
    double prev_output;
};

static double
filter_dc(struct dc_filter_s* self, double sample)
{
    double alpha = 0.9957;
    double output = sample - self->prev_input + alpha * self->prev_output;
    self->prev_input = sample;
    self->prev_output = output;
    return output;
}
