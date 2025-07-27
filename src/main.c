/* c23 standard lib */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <float.h>
#include <math.h>
#include <threads.h>

/* engine sim common defs */
#include "std.h"

/* signal processing */
#include "moving_average_filter_s.h"
#include "envelope_s.h"
#include "normalized_s.h"
#include "gain_filter_s.h"
#include "convo_filter_s.h"
#include "lowpass_filter_s.h"
#include "highpass_filter_s.h"

/* engine fluids */
#include "gamma.h"
#include "gas_s.h"
#include "chamber_s.h"
#include "gas_mail_s.h"
#include "nozzle_flow_s.h"
#include "visualize.h"

/* engine peripherals */
#include "crankshaft_s.h"
#include "flywheel_s.h"
#include "starter_s.h"
#include "valve_s.h"

/* engine synth */
#include "synth_s.h"
#include "wave_s.h"

/* engine nodes */
#include "source_s.h"
#include "filter_s.h"
#include "iplenum_s.h"
#include "throttle_s.h"
#include "irunner_s.h"
#include "piston_s.h"
#include "erunner_s.h"
#include "eplenum_s.h"
#include "exhaust_s.h"
#include "sink_s.h"
#include "node_s.h"

/* engine data collection */
#include "sampler_s.h"

/* engine assembly */
#include "engine_s.h"
#include "engine_1_cyl.h"
#include "engine_8_cyl.h"

/* sdl3 (video, input, and audio access) + helpers */
#include <SDL3/SDL.h>
#include "sdl_scroll_s.h"
#include "sdl_slide_buffer_t.h"
#include "sdl_time_panel_s.h"
#include "sdl_panel_s.h"
#include "sdl_progress_bar_s.h"
#include "sdl_ui.h"
#include "sdl.h"
#include "sdl_audio.h"

static double
get_ticks_ms()
{
    double ticks_ns = SDL_GetTicksNS();
    return SDL_NS_TO_MS(ticks_ns);
}

static struct sampler_s g_sampler;

static sampler_synth_t g_sampler_synth;

int
main(int argc, char* argv[])
{
    size_t cycles = argc == 2 ? atoi(argv[1]) : -1;
    init_cp_precompute_buffer();
#ifdef ENSIM4_VISUALIZE
    visualize_gamma();
    visualize_chamber_s();
#endif
    struct engine_s* engine = &g_engine_1_cyl;
    reset_engine(engine);
    init_sdl();
    init_sdl_audio();
    for(size_t cycle = 0; cycle < cycles; cycle++)
    {
        struct engine_time_s engine_time = {
            .get_ticks_ms = get_ticks_ms
        };
        double t0 = get_ticks_ms();
        /* for something soon */
        double t1 = get_ticks_ms();
        g_r_per_s_progress_bar.value = engine->crankshaft.angular_velocity_r_per_s;
        g_synth_envelope_progress_bar.value = g_synth.envelope.gain;
        clear_synth(&g_synth);
        size_t audio_buffer_size = get_audio_buffer_size();
        run_engine(engine, &engine_time, &g_sampler, &g_synth, audio_buffer_size, g_sampler_synth);
        buffer_audio(&g_synth);
        double t2 = get_ticks_ms();
        if(handle_input(engine, &g_sampler))
        {
            break;
        }
        draw_everything(
            engine,
            &g_sampler,
            &g_loop_time_panel,
            &g_engine_time_panel,
            &g_audio_buffer_time_panel,
            &g_r_per_s_progress_bar,
            &g_frames_per_sec_progress_bar,
            &g_synth_envelope_progress_bar,
            &g_starter_panel_r_per_s,
            &g_convolution_panel_time_domain,
            g_wave_panel,
            g_wave_panel_size,
            &g_synth_sample_panel);
        double t3 = get_ticks_ms();
        present(0.0);
        double t4 = get_ticks_ms();
        push_panels(engine, &engine_time, &g_sampler, g_sampler_synth, audio_buffer_size, t0, t1, t2, t3, t4);
        double t5 = get_ticks_ms();
        g_frames_per_sec_progress_bar.value = 1000.0 / (t5 - t0);
    }
    exit_sdl_audio();
    exit_sdl();
}
