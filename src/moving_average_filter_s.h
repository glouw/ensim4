static constexpr size_t g_moving_average_filter_size = 64;

struct moving_average_filter_s
{
    double sample[g_moving_average_filter_size];
    size_t index;
};

static double
push_moving_average_filter(struct moving_average_filter_s* self, double value)
{
    self->sample[self->index++] = value;
    self->index %= g_moving_average_filter_size;
    double sum = 0.0;
    for(size_t i = 0; i < g_moving_average_filter_size; i++)
    {
        sum += self->sample[i];
    }
    return sum / g_moving_average_filter_size;
}
