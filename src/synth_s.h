static constexpr size_t g_synth_buffer_size = g_std_audio_sample_rate_hz / g_std_assumed_refresh_rate;
static constexpr size_t g_synth_buffer_min_size = 1 * g_synth_buffer_size;
static constexpr size_t g_synth_buffer_mid_size = 4 * g_synth_buffer_size;
static constexpr size_t g_synth_buffer_max_size = 7 * g_synth_buffer_size;

struct synth_s
{
    struct dc_filter_s dc_filter;
    struct convo_s convo;
    float value[g_synth_buffer_size];
    size_t index;
};

static void
normalize_synth(struct synth_s* self)
{
    normalize_samples(self->value, self->index);
}

static void
sample_synth(struct synth_s* self, double value)
{
    self->value[self->index] += value;
}

static void
step_synth(struct synth_s* self)
{
    self->index += 1;
}

static void
clear_synth(struct synth_s* self)
{
    self->index = 0;
    clear(self->value);
}

static double
push_synth(struct synth_s* self, double sample)
{
    sample = filter_dc(&self->dc_filter, sample);
    sample = filter_convo(&self->convo, sample);
    sample_synth(self, sample);
    step_synth(self);
    return sample;
}
