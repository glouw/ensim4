static struct synth_s g_synth = {
    .envelope = {
        .max_gain = g_synth_max_gain,
        .limiter = g_synth_limiter,
    }
};

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
    .rect.w = 192,
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
    .rect.w = 192,
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
    .rect.w = 192,
    .rect.h = 96,
};

static struct sdl_progress_bar_s g_r_per_s_progress_bar = {
    .title = "crank_r_per_s",
    .max_value = 2000.0,
    .rect.w = 192,
    .rect.h = 16,
};

static struct sdl_progress_bar_s g_frames_per_sec_progress_bar = {
    .title = "frames_per_sec",
    .max_value = 100.0,
    .rect.w = 192,
    .rect.h = 16,
};

static struct sdl_progress_bar_s g_synth_envelope_progress_bar = {
    .title = "synth_envelope",
    .max_value = 0.25,
    .rect.w = 192,
    .rect.h = 16,
};

static struct sdl_panel_s g_starter_panel_r_per_s = {
    .title = "starter_r_per_s",
    .rect.w = 192,
    .rect.h = 64,
};

static struct sdl_panel_s g_convolution_panel_time_domain = {
    .title = "convolution (time)",
    .rect.w = 192,
    .rect.h = 64,
};

static struct sdl_panel_s g_wave_panel[] = {
    { .title = "hllc_wave_0_pa", .rect.w = 192, .rect.h = 128 },
    { .title = "hllc_wave_1_pa", .rect.w = 192, .rect.h = 128 },
    { .title = "hllc_wave_2_pa", .rect.w = 192, .rect.h = 128 },
    { .title = "hllc_wave_3_pa", .rect.w = 192, .rect.h = 128 },
};

static constexpr size_t g_wave_panel_size = len(g_wave_panel);

static struct sdl_panel_s g_synth_sample_panel = {
    .title = "synth_samples",
    .rect.w = 192,
    .rect.h = 64,
};

static void
push_panels(
    struct engine_s* engine,
    struct engine_time_s* engine_time,
    struct sampler_s* sampler,
    sampler_synth_t sampler_synth,
    size_t audio_buffer_size,
    double t0,
    double t1,
    double t2,
    double t3,
    double t4)
{
    push_panel(&g_starter_panel_r_per_s, sampler->starter, sampler->size);
    if(engine->use_convolution)
    {
        push_panel_double(&g_convolution_panel_time_domain, g_convo_filter_impulse, g_convo_filter_impulse_size);
    }
    else
    {
        clear_panel(&g_convolution_panel_time_domain);
    }
    push_panel(&g_synth_sample_panel, sampler_synth, g_synth_buffer_size);
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
    push_time_panel(
        &g_loop_time_panel,
        (float[]) {
            t1 - t0,
            t2 - t1,
            t3 - t2,
            t4 - t0,
        }
    );
}
