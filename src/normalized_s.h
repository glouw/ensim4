struct normalized_s
{
    double max_value;
    double avg_value;
    double min_value;
    double div_value;
    bool is_success;
};

static struct normalized_s
normalize_samples(double samples[], size_t size)
{
    struct normalized_s normalized = {
        .max_value = -FLT_MAX,
        .min_value = +FLT_MAX,
        .is_success = false,
    };
    for(size_t i = 0; i < size; i++)
    {
        normalized.max_value = max(normalized.max_value, samples[i]);
    }
    for(size_t i = 0; i < size; i++)
    {
        normalized.avg_value += samples[i];
    }
    normalized.avg_value /= size;
    for(size_t i = 0; i < size; i++)
    {
        normalized.min_value = min(normalized.min_value, samples[i]);
    }
    double range = normalized.max_value - normalized.min_value;
    if(range < 1e-9)
    {
        return normalized;
    }
    normalized.div_value = normalized.max_value / normalized.min_value;
    for(size_t i = 0; i < size; i++)
    {
        samples[i] = (samples[i] - normalized.min_value) / range;
    }
    normalized.is_success = true;
    return normalized;
}

static double
calc_normalized_zero_offset_ratio(struct normalized_s* self)
{
    return self->max_value / (self->max_value - self->min_value);
}
