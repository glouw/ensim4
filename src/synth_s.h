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
clamp_synth(struct synth_s* self, double value)
{
    return clamp(value, -1.0, 1.0);
}

static double
adjust_gain_synth(struct synth_s* self, double value)
{
    value /= 5e4;
    return value;
}

static double
push_synth(struct synth_s* self, double value)
{
    value = filter_dc(&self->dc_filter, value);
    value = filter_convo(&self->convo, value);
    value = adjust_gain_synth(self, value);
    value = clamp_synth(self, value);
    sample_synth(self, value);
    step_synth(self);
    return value;
}
