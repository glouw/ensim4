static constexpr size_t g_synth_buffer_size = g_std_audio_sample_rate_hz / g_std_assumed_refresh_rate;
static constexpr size_t g_synth_buffer_min_size = 1 * g_synth_buffer_size;
static constexpr size_t g_synth_buffer_mid_size = 5 * g_synth_buffer_size;
static constexpr size_t g_synth_buffer_max_size = 7 * g_synth_buffer_size;

struct synth_s
{
    float value[g_synth_buffer_size];
    size_t index;
};

static void
sample_synth(struct synth_s* self, float value)
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
