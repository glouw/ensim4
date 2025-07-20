static constexpr size_t g_synth_buffer_forgiveness_size = 32;
static constexpr size_t g_synth_buffer_size = g_std_audio_sample_rate_hz / g_std_assumed_refresh_rate + g_synth_buffer_forgiveness_size;
static constexpr size_t g_synth_buffer_min_size = 1 * g_synth_buffer_size;
static constexpr size_t g_synth_buffer_mid_size = 4 * g_synth_buffer_size;
static constexpr size_t g_synth_buffer_max_size = 5 * g_synth_buffer_size;
static constexpr double g_dc_filter_cutoff_frequency_hz = 10.0;

struct synth_s
{
    struct highpass_filter_s dc_filter;
    struct gain_filter_s gain_filter;
    struct convo_filter_s convo_filter;
    double clamp;
    float value[g_synth_buffer_size];
    size_t index;
};

static void
sample_synth(struct synth_s* self, double value)
{
    self->value[self->index++] += value;
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
    return clamp(value, -self->clamp, self->clamp);
}

static double
push_synth(struct synth_s* self, struct crankshaft_s* crankshaft, double value)
{
    value = filter_highpass(&self->dc_filter, g_dc_filter_cutoff_frequency_hz, value);
    value = filter_convo(&self->convo_filter, value);
    value = filter_gain(&self->gain_filter, value);
    value = clamp_synth(self, value);
    sample_synth(self, value);
    return value;
}
