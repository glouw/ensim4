static constexpr size_t g_moving_average_filter_size = 64;

struct moving_average_filter_s
{
    double sample[g_moving_average_filter_size];
    size_t index;
    bool is_full;
};

static double
push_moving_average_filter(struct moving_average_filter_s* self, double value)
{
    self->sample[self->index++] = value;
    if(self->index == g_moving_average_filter_size)
    {
        self->is_full = true;
    }
    self->index %= g_moving_average_filter_size;
    size_t count = self->is_full ? g_moving_average_filter_size : self->index;
    double sum = 0.0;
    for(size_t i = 0; i < count; i++)
    {
        sum += self->sample[i];
    }
    return sum / count;
}
