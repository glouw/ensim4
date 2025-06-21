#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <float.h>
#include <math.h>

/* engine sim common defs */
#include "std.h"

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

/* engine synth */
#include "synth_s.h"
#include "sampler_s.h"

/* engine assembly */
#include "engine_s.h"
#include "engine_8_cyl.h"

/* sdl3 (video, input, and audio access) */
#include <SDL3/SDL.h>
#include "normalized_s.h"
#include "sdl_scroll_s.h"
#include "sdl_slide_buffer_t.h"
#include "sdl_time_panel_s.h"
#include "sdl_progress_bar_s.h"
#include "sdl.h"
#include "sdl_audio.h"

static struct sampler_s g_sampler = {};
static struct synth_s g_synth = {};

static struct sdl_time_panel_s g_loop_time_panel = {
    .title = "loop_time_ms",
    .labels = {
        "engine",
        "input",
        "draw",
        "vsync",
    },
    .min_value = 0.0,
    .max_value = 40.0,
    .rect.w = 192,
    .rect.h = 96,
};

static struct sdl_time_panel_s g_engine_time_panel = {
    .title = "engine_time_ms",
    .labels = {
        "fluids",
        "thermo",
        "TBD",
        "kinematics",
    },
    .min_value = 0.0,
    .max_value = 40.0,
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
    .title = "radians_per_second",
    .max_value = 2000.0,
    .rect.w = 192,
    .rect.h = 16,
};

static struct sdl_progress_bar_s g_frames_per_sec_bar = {
    .title = "frames_per_sec",
    .max_value = 100.0,
    .rect.w = 192,
    .rect.h = 16,
};

int
main(int argc, char* argv[])
{
    init_cp_precompute_buffer();
    visualize_gamma();
    visualize_chamber_s();
    size_t cycles = argc == 2 ? atoi(argv[1]) : -1;
    struct engine_s* engine = &g_engine_8_cyl;
    reset_engine(engine);
    init_sdl();
    init_sdl_audio();
    for(size_t cycle = 0; cycles == (size_t) -1 ? true : cycle < cycles; cycle++)
    {
        struct engine_time_s engine_time = { .get_ms = SDL_GetTicks };
        size_t t0 = SDL_GetTicks();
        clear_synth(&g_synth);
        size_t g_audio_buffer_size = get_audio_buffer_size();
        run_engine(engine, &engine_time, &g_sampler, &g_synth, g_audio_buffer_size);
        g_r_per_s_progress_bar.value = engine->crankshaft.angular_velocity_r_per_s;
        buffer_audio(&g_synth);
        push_time_panel(
            &g_engine_time_panel,
            (float[]) {
                engine_time.fluids_time_ms,
                engine_time.thermo_time_ms,
                engine_time.tbd_time_ms,
                engine_time.kinematics_time_ms,
            }
        );
        push_time_panel(
            &g_audio_buffer_time_panel,
            (float[]) {
                g_audio_buffer_size,
                g_synth_buffer_min_size,
                0.0,
                0.0,
            }
        );
        size_t t1 = SDL_GetTicks();
        if(handle_input(engine, &g_sampler))
        {
            break;
        }
        size_t t2 = SDL_GetTicks();
        clear_screen();
        draw_plots(engine, &g_sampler);
        draw_radial_chambers(engine);
        draw_info(
            engine,
            &g_loop_time_panel,
            &g_engine_time_panel,
            &g_audio_buffer_time_panel,
            &g_r_per_s_progress_bar,
            &g_frames_per_sec_bar);
        size_t t3 = SDL_GetTicks();
        present(0.0);
        size_t t4 = SDL_GetTicks();
        push_time_panel(
            &g_loop_time_panel,
            (float[]) {
                t1 - t0,
                t2 - t1,
                t3 - t2,
                t4 - t0,
            }
        );
        size_t t5 = SDL_GetTicks();
        g_frames_per_sec_bar.value = 1000.0 / (t5 - t0);
    }
    exit_sdl_audio();
    exit_sdl();
}
