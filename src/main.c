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
#include "filter_s.h"
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
#include "engine_2_cyl.h"
#include <SDL3/SDL.h>
#include "sdl_scroll_s.h"
#include "sdl_slide_buffer_t.h"
#include "sdl_time_panel_s.h"
#include "sdl_panel_s.h"
#include "sdl_progress_bar_s.h"
#include "sdl.h"
#include "sdl_widgets.h"
#include "sdl_audio.h"

double
get_ticks_ms()
{
    double ticks_ns = SDL_GetTicksNS();
    return SDL_NS_TO_MS(ticks_ns);
}

struct sampler_s g_sampler = {};

sampler_synth_t g_sampler_synth = {};

struct synth_s g_synth = {};

struct engine_s g_engine = {
    .name = g_engine_name,
    engine_is(g_engine_node),
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
            g_wave_panel_size,
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
