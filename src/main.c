#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <float.h>
#include <math.h>
#include "std.h"

/* fluid mechanics */
#include "gamma.h"
#include "gas_s.h"
#include "chamber_s.h"
#include "gas_mail_s.h"
#include "nozzle_flow_s.h"
#include "visualize.h"

/* volumes and mechanics */
#include "source_s.h"
#include "filter_s.h"
#include "iplenum_s.h"
#include "throttle_s.h"
#include "irunner_s.h"
#include "crankshaft_s.h"
#include "flywheel_s.h"
#include "piston_s.h"
#include "erunner_s.h"
#include "eplenum_s.h"
#include "exhaust_s.h"
#include "sink_s.h"
#include "node_s.h"
#include "starter_s.h"
#include "sampler_s.h"
#include "synth_s.h"
#include "engine_s.h"
#include "engine_8_cyl.h"

/* display and input */
#include <SDL3/SDL.h>
#include "normalized_s.h"
#include "sdl_scroll_s.h"
#include "sdl_slide_buffer_t.h"
#include "sdl_time_panel_s.h"
#include "sdl_progress_bar_s.h"
#include "sdl.h"
#include "sdl_audio.h"

static struct sampler_s sampler = {};
static struct synth_s synth = {};

static struct sdl_time_panel_s loop_time_panel = {
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

static struct sdl_time_panel_s engine_time_panel = {
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

static struct sdl_time_panel_s audio_buffer_time_panel = {
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

int
main(int argc, char* argv[])
{
    struct engine_s* engine = &g_engine_8_cyl;
    double frames_per_sec = 0.0;
    struct sdl_progress_bar_s r_per_s_progress_bar = {
        .title = "radians_per_second",
        .max_value = 2000.0,
        .value = &engine->crankshaft.angular_velocity_r_per_s,
        .rect.w = 192,
        .rect.h = 16,
    };
    struct sdl_progress_bar_s frames_per_sec_bar = {
        .title = "frames_per_sec",
        .max_value = 100.0,
        .value = &frames_per_sec,
        .rect.w = 192,
        .rect.h = 16,
    };
    init_cp_precompute_buffer();
    visualize_gamma();
    visualize_chamber_s();
    size_t cycles = argc == 2 ? atoi(argv[1]) : -1;
    reset_engine(engine);
    init_sdl();
    init_sdl_audio();
    for(size_t cycle = 0; cycles == (size_t) -1 ? true : cycle < cycles; cycle++)
    {
        struct engine_time_s engine_time = {
            .get_ms = SDL_GetTicks
        };
        size_t t0 = SDL_GetTicks();
        clear_synth(&synth);
        size_t audio_buffer_size = get_audio_buffer_size();
        run_engine(engine, &engine_time, &sampler, &synth, audio_buffer_size);
        buffer_audio(&synth);
        push_time_panel(
            &engine_time_panel,
            (float[]) {
                engine_time.fluids_time_ms,
                engine_time.thermo_time_ms,
                engine_time.tbd_time_ms,
                engine_time.kinematics_time_ms,
            }
        );
        push_time_panel(
            &audio_buffer_time_panel,
            (float[]) {
                audio_buffer_size,
                g_synth_buffer_min_size,
                0.0,
                0.0,
            }
        );
        size_t t1 = SDL_GetTicks();
        if(handle_input(engine, &sampler))
        {
            break;
        }
        size_t t2 = SDL_GetTicks();
        clear_screen();
        draw_plots(engine, &sampler);
        draw_radial_chambers(engine);
        draw_info(
            engine,
            &loop_time_panel,
            &engine_time_panel,
            &audio_buffer_time_panel,
            &r_per_s_progress_bar,
            &frames_per_sec_bar);
        size_t t3 = SDL_GetTicks();
        present(0.0);
        size_t t4 = SDL_GetTicks();
        push_time_panel(
            &loop_time_panel,
            (float[]) {
                t1 - t0,
                t2 - t1,
                t3 - t2,
                t4 - t0,
            }
        );
        size_t t5 = SDL_GetTicks();
        frames_per_sec = 1000.0 / (t5 - t0);
    }
    exit_sdl_audio();
    exit_sdl();
}
