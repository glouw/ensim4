static constexpr size_t g_synth_buffer_size = g_std_audio_sample_rate_hz / g_std_monitor_refresh_rate;
static constexpr size_t g_synth_buffer_min_size = 1 * g_synth_buffer_size;
static constexpr size_t g_synth_buffer_max_size = 5 * g_synth_buffer_size;
static constexpr double g_synth_dc_filter_cutoff_frequency_hz = 10.0;
static constexpr double g_synth_deadzone_angular_velocity_r_per_s = 1.0;
static constexpr double g_synth_clamp = 1.0;
static constexpr double g_synth_max_gain = 0.5;
static constexpr double g_synth_angular_velocity_limiter_r_per_s = 200.0;

struct synth_s
{
    struct highpass_filter_s dc_filter;
    struct gain_filter_s gain_filter;
    struct convo_filter_s convo_filter;
    struct envelope_s envelope;
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
clamp_synth(double value)
{
    return clamp(value, -g_synth_clamp, g_synth_clamp);
}

static double
set_synth_deadzone(double value, struct crankshaft_s* crankshaft)
{
    double absolute_angular_velocity_r_per_s = fabs(crankshaft->angular_velocity_r_per_s);
    if(absolute_angular_velocity_r_per_s < g_synth_deadzone_angular_velocity_r_per_s)
    {
        return 0.0;
    }
    return value;
}

static double
push_synth(struct synth_s* self, struct crankshaft_s* crankshaft, double value, bool use_convolution)
{
    process_envelope(&self->envelope, crankshaft->angular_velocity_r_per_s);
    value = filter_highpass(&self->dc_filter, g_synth_dc_filter_cutoff_frequency_hz, value);
    if(use_convolution)
    {
        value = filter_convo(&self->convo_filter, value);
    }
    value = filter_gain(&self->gain_filter, value, self->envelope.gain);
    value = set_synth_deadzone(value, crankshaft);
    value = clamp_synth(value);
    sample_synth(self, value);
    return value;
}
