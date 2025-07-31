static struct sdl_time_panel_s g_loop_time_panel = {
    .title = "loop_time_ms",
    .labels = {
        "n/a",
        "engine",
        "draw",
        "vsync",
    },
    .min_value = 0.0,
    .max_value = 20.0,
    .rect.w = g_sdl_supported_widget_w_p,
    .rect.h = 96,
};

static struct sdl_time_panel_s g_engine_time_panel = {
    .title = "engine_time_ms",
    .labels = {
        "fluids",
        "kinematics",
        "thermo",
        "synth",
        "waves",
    },
    .min_value = 0.0,
    .max_value = 15.0,
    .rect.w = g_sdl_supported_widget_w_p,
    .rect.h = 96,
};

static struct sdl_time_panel_s g_audio_buffer_time_panel = {
    .title = "audio_buffer_size",
    .labels = {
        "buffer_size",
        "min_size",
        nullptr,
        nullptr,
    },
    .min_value = 0.0,
    .max_value = g_synth_buffer_max_size,
    .rect.w = g_sdl_supported_widget_w_p,
    .rect.h = 96,
};

static struct sdl_progress_bar_s g_r_per_s_progress_bar = {
    .title = "crank_r_per_s",
    .max_value = 2000.0,
    .rect.w = g_sdl_supported_widget_w_p,
    .rect.h = 16,
};

static struct sdl_progress_bar_s g_frames_per_sec_progress_bar = {
    .title = "frames_per_sec",
    .max_value = 100.0,
    .rect.w = g_sdl_supported_widget_w_p,
    .rect.h = 16,
};

static struct sdl_progress_bar_s g_synth_envelope_progress_bar = {
    .title = "synth_envelope",
    .max_value = 0.25,
    .rect.w = g_sdl_supported_widget_w_p,
    .rect.h = 16,
};

static struct sdl_panel_s g_starter_panel_r_per_s = {
    .title = "starter_r_per_s",
    .rect.w = g_sdl_supported_widget_w_p,
    .rect.h = 64,
};

static struct sdl_panel_s g_convolution_panel_time_domain = {
    .title = "impulse x[n]",
    .rect.w = g_sdl_supported_widget_w_p,
    .rect.h = 64,
};

static struct sdl_panel_s g_wave_panel[] = {
    { .title = "hllc_wave_0_pa", .rect.w = g_sdl_supported_widget_w_p, .rect.h = 32 },
    { .title = "hllc_wave_1_pa", .rect.w = g_sdl_supported_widget_w_p, .rect.h = 32 },
    { .title = "hllc_wave_2_pa", .rect.w = g_sdl_supported_widget_w_p, .rect.h = 32 },
    { .title = "hllc_wave_3_pa", .rect.w = g_sdl_supported_widget_w_p, .rect.h = 32 },
};

static constexpr size_t g_wave_panel_size = len(g_wave_panel);

static struct sdl_panel_s g_synth_sample_panel = {
    .title = "synth_samples",
    .rect.w = g_sdl_supported_widget_w_p,
    .rect.h = 64,
};

struct widget_time_s
{
    double n_a_time_ms;
    double engine_time_ms;
    double draw_time_ms;
    double vsync_time_ms;
    double (*get_ticks_ms)();
};

static void
push_widgets(
    struct engine_s* engine,
    struct engine_time_s* engine_time,
    struct sampler_s* sampler,
    struct synth_s* synth,
    sampler_synth_t sampler_synth,
    size_t audio_buffer_size,
    struct widget_time_s* widget_time)
{
    push_time_panel(
        &g_loop_time_panel,
        (float[]) {
            widget_time->n_a_time_ms,
            widget_time->engine_time_ms,
            widget_time->draw_time_ms,
            widget_time->vsync_time_ms,
        }
    );
    push_time_panel(
        &g_engine_time_panel,
        (float[]) {
            engine_time->fluids_time_ms,
            engine_time->kinematics_time_ms,
            engine_time->thermo_time_ms,
            engine_time->synth_time_ms,
            engine_time->wave_time_ms,
        }
    );
    push_time_panel(
        &g_audio_buffer_time_panel,
        (float[]) {
            audio_buffer_size,
            g_synth_buffer_min_size,
            0.0,
            0.0,
        }
    );
    g_r_per_s_progress_bar.value = engine->crankshaft.angular_velocity_r_per_s;
    g_synth_envelope_progress_bar.value = synth->envelope.gain;
    g_frames_per_sec_progress_bar.value = 1000.0 / widget_time->vsync_time_ms;
    push_panel(&g_starter_panel_r_per_s, sampler->starter, sampler->size);
    if(engine->use_convolution)
    {
        push_panel_double(
            &g_convolution_panel_time_domain,
            g_convo_filter_impulse,
            g_convo_filter_impulse_size);
    }
    else
    {
        clear_panel(&g_convolution_panel_time_domain);
    }
    push_panel(
        &g_synth_sample_panel,
        sampler_synth,
        g_synth_buffer_size);
}
