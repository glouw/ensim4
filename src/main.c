#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <float.h>
#include <math.h>
#include <threads.h>

#include "std.h"
#include "normalized_s.h"
#include "convo_filter_s.h"
#include "highpass_filter_s.h"
#include "lowpass_filter_s.h"
#include "gamma.h"
#include "gas_s.h"
#include "chamber_s.h"
#include "gas_mail_s.h"
#include "nozzle_flow_s.h"
#include "visualize.h"
#include "crankshaft_s.h"
#include "sparkplug_s.h"
#include "flywheel_s.h"
#include "starter_s.h"
#include "limiter_s.h"
#include "valve_s.h"
#include "synth_s.h"
#include "wave_s.h"
#include "source_s.h"
#include "afilter_s.h"
#include "iplenum_s.h"
#include "injector_s.h"
#include "throttle_s.h"
#include "irunner_s.h"
#include "piston_s.h"
#include "erunner_s.h"
#include "eplenum_s.h"
#include "exhaust_s.h"
#include "sink_s.h"
#include "node_s.h"
#include "sampler_s.h"

#include "engine_s.h"
#include "engine_blueprints.h"
#ifdef ENGINE_2_CYL
  #include "engine_2_cyl.h"
#elifdef ENGINE_3_CYL
  #include "engine_3_cyl.h"
#elifdef ENGINE_8_CYL
  #include "engine_8_cyl.h"
#endif

#include <SDL3/SDL.h>
#include "sdl_scroll_s.h"
#include "sdl_slide_buffer_t.h"
#include "sdl_time_panel_s.h"
#include "sdl_panel_s.h"
#include "sdl_progress_bar_s.h"
#include "sdl.h"
#include "sdl_audio.h"

struct sampler_s g_sampler = {};
sampler_synth_t g_sampler_synth = {};
struct synth_s g_synth = {};

struct sdl_time_panel_s g_loop_time_panel = {
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

struct sdl_time_panel_s g_engine_time_panel = {
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

struct sdl_time_panel_s g_audio_buffer_time_panel = {
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

struct sdl_progress_bar_s g_r_per_s_progress_bar = {
    .title = "crank_r_per_s",
    .max_value = 2000.0,
    .rect.w = g_sdl_supported_widget_w_p,
    .rect.h = 16,
};

struct sdl_progress_bar_s g_frames_per_sec_progress_bar = {
    .title = "frames_per_sec",
    .max_value = 100.0,
    .rect.w = g_sdl_supported_widget_w_p,
    .rect.h = 16,
};

struct sdl_progress_bar_s g_throttle_progress_bar = {
    .title = "throttle",
    .max_value = 1.0,
    .rect.w = g_sdl_supported_widget_w_p,
    .rect.h = 16,
};

struct sdl_panel_s g_starter_panel_r_per_s = {
    .title = "starter_r_per_s",
    .rect.w = g_sdl_supported_widget_w_p,
    .rect.h = 64,
};

struct sdl_panel_s g_convolution_panel_time_domain = {
    .title = "impulse x[n]",
    .rect.w = g_sdl_supported_widget_w_p,
    .rect.h = 64,
};

struct sdl_panel_s g_wave_panel[g_wave_max_waves] = {
    { .title = "wave_0_pa", .rect.w = g_sdl_supported_widget_w_p, .rect.h = 48 },
    { .title = "wave_1_pa", .rect.w = g_sdl_supported_widget_w_p, .rect.h = 48 },
    { .title = "wave_2_pa", .rect.w = g_sdl_supported_widget_w_p, .rect.h = 48 },
    { .title = "wave_3_pa", .rect.w = g_sdl_supported_widget_w_p, .rect.h = 48 },
};

struct sdl_panel_s g_synth_sample_panel = {
    .title = "synth_samples",
    .rect.w = g_sdl_supported_widget_w_p,
    .rect.h = 64,
};

struct engine_s g_engine = {
    .name = g_engine_name,
    .node = g_engine_node,
    .size = len(g_engine_node),
    .crankshaft = {
        .mass_kg = g_engine_crankshaft_mass_kg,
        .radius_m = g_engine_crankshaft_radius_m,
    },
    .flywheel = {
        .mass_kg = g_engine_flywheel_mass_kg,
        .radius_m = g_engine_flywheel_radius_m,
    },
    .limiter = {
        .cutoff_angular_velocity_r_per_s = g_engine_limiter_cutoff_r_per_s,
        .relaxed_angular_velocity_r_per_s = g_engine_limiter_relaxed_r_per_s,
    },
    .starter = {
        .rated_torque_n_m = e_engine_starter_rated_torque_n_m,
        .no_load_angular_velocity_r_per_s = g_engine_starter_no_load_r_per_s,
        .radius_m = g_engine_starter_radius_m,
    },
    .volume = g_engine_sound_volume,
    .no_throttle = g_engine_no_throttle,
    .low_throttle = g_engine_low_throttle,
    .mid_throttle = g_engine_mid_throttle,
    .high_throttle = g_engine_high_throttle,
    .radial_spacing = g_engine_radial_spacing,
};

double
get_ticks_ms()
{
    double ticks_ns = SDL_GetTicksNS();
    return SDL_NS_TO_MS(ticks_ns);
}

struct widget_time_s
{
    double n_a_time_ms;
    double engine_time_ms;
    double draw_time_ms;
    double vsync_time_ms;
    double (*get_ticks_ms)();
};

void
push_widgets(
    struct engine_s* engine,
    struct engine_time_s* engine_time,
    struct sampler_s* sampler,
    sampler_synth_t sampler_synth,
    size_t audio_buffer_size,
    struct widget_time_s* widget_time)
{
    push_time_panel(
        &g_loop_time_panel,
        (double[]) {
            widget_time->n_a_time_ms,
            widget_time->engine_time_ms,
            widget_time->draw_time_ms,
            widget_time->vsync_time_ms,
        }
    );
    push_time_panel(
        &g_engine_time_panel,
        (double[]) {
            engine_time->fluids_time_ms,
            engine_time->kinematics_time_ms,
            engine_time->thermo_time_ms,
            engine_time->synth_time_ms,
            engine_time->wave_time_ms,
        }
    );
    push_time_panel(
        &g_audio_buffer_time_panel,
        (double[]) {
            audio_buffer_size,
            g_synth_buffer_min_size,
            0.0,
            0.0,
        }
    );
    g_r_per_s_progress_bar.value = engine->crankshaft.angular_velocity_r_per_s;
    g_throttle_progress_bar.value = engine->throttle_open_ratio;
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

int
main()
{
    precompute_cp();
#ifdef ENSIM4_VISUALIZE
    visualize_gamma();
    visualize_chamber_s();
#endif
    reset_engine(&g_engine);
    init_sdl();
    init_sdl_audio();
#ifdef ENSIM4_PERF
    size_t perf_max_cycles = 360;
    for(size_t cycle = 0; cycle < perf_max_cycles; cycle++)
#else
    for(;;)
#endif
    {
        struct engine_time_s engine_time = { .get_ticks_ms = get_ticks_ms };
        struct widget_time_s widget_time = { .get_ticks_ms = get_ticks_ms };
        double t0 = widget_time.get_ticks_ms();
        double t1 = widget_time.get_ticks_ms();
        clear_synth(&g_synth);
        size_t audio_buffer_size = get_audio_buffer_size();
        run_engine(
            &g_engine,
            &engine_time,
            &g_sampler,
            &g_synth,
            audio_buffer_size,
            g_sampler_synth);
        buffer_audio(&g_synth);
        double t2 = widget_time.get_ticks_ms();
        if(handle_input(&g_engine, &g_sampler))
        {
            break;
        }
        draw_to_renderer(
            &g_engine,
            &g_sampler,
            &g_loop_time_panel,
            &g_engine_time_panel,
            &g_audio_buffer_time_panel,
            &g_r_per_s_progress_bar,
            &g_frames_per_sec_progress_bar,
            &g_throttle_progress_bar,
            &g_starter_panel_r_per_s,
            &g_convolution_panel_time_domain,
            g_wave_panel,
            len(g_wave_panel),
            &g_synth_sample_panel);
        double t3 = widget_time.get_ticks_ms();
        present_renderer();
        double t4 = widget_time.get_ticks_ms();
        widget_time.n_a_time_ms = t1 - t0;
        widget_time.engine_time_ms = t2 - t1;
        widget_time.draw_time_ms = t3 - t2;
        widget_time.vsync_time_ms = t4 - t0;
        push_widgets(
            &g_engine,
            &engine_time,
            &g_sampler,
            g_sampler_synth,
            audio_buffer_size,
            &widget_time);
    }
    exit_sdl_audio();
    exit_sdl();
}
