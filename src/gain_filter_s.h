static constexpr size_t g_gain_filter_size = 8192;
static constexpr double g_gain_filter_pre_gain = 0.1;

struct gain_filter_s
{
    double buffer[g_gain_filter_size];
    size_t index;
    double sum_of_squares;
};

static double
filter_gain(struct gain_filter_s* self, double value)
{
    double old_value = self->buffer[self->index];
    self->buffer[self->index++] = value;
    self->index %= g_gain_filter_size;
    self->sum_of_squares -= old_value * old_value;
    self->sum_of_squares += value * value;
    if(self->sum_of_squares < 0.0)
    {
        self->sum_of_squares = 0.0;
    }
    double root_mean_square = 0.0;
    if(self->sum_of_squares > 0.0)
    {
        root_mean_square = sqrt(self->sum_of_squares / g_gain_filter_size);
    }
    value *= g_gain_filter_pre_gain;
    if(root_mean_square > 0.0)
    {
        value /= root_mean_square;
    }
    return value;
}
