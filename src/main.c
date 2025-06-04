#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <float.h>
#include <math.h>
#include "std.h"
#include "gamma.h"
#include "gas_s.h"
#include "chamber_s.h"
#include "gas_mail_s.h"
#include "source_s.h"
#include "filter_s.h"
#include "iplenum_s.h"
#include "throttle_s.h"
#include "irunner_s.h"
#include "crankshaft_s.h"
#include "piston_s.h"
#include "erunner_s.h"
#include "eplenum_s.h"
#include "exhaust_s.h"
#include "sink_s.h"
#include "nozzle_flow_s.h"
#include "node_s.h"
#include "sample_s.h"
#include "engine_s.h"
#include "visualize.h"

/* isolated from engine core */
#include <SDL3/SDL.h>
#include "sdl_scroll_s.h"
#include "sdl_slide_buffer_t.h"
#include "sdl_time_panel_s.h"
#include "sdl_progress_bar_s.h"
#include "sdl_normalized_s.h"
#include "sdl.h"

static struct engine_s engine;

static struct sdl_time_panel_s loop_time_panel = {
    .title = "loop_time_ms",
    .labels = {
        "engine",
        "input",
        "draw",
        "vsync",
    },
    .min_time_ms = 0.0,
    .max_time_ms = 25.0,
    .rect.w = 192,
    .rect.h = 128,
};

static struct sdl_time_panel_s engine_time_panel = {
    .title = "engine_time_ms",
    .labels = {
        "fluids",
        "thermo",
        "N/A",
        "kinematics",
    },
    .min_time_ms = 0.0,
    .max_time_ms = 16.0,
    .rect.w = 192,
    .rect.h = 192,
};

static struct sdl_progress_bar_s rad_per_sec_progress_bar = {
    .title = "rad_per_sec",
    .max_value = 2000.0,
    .value = &engine.crankshaft.angular_velocity_r_per_s,
    .rect.w = 192,
    .rect.h = 32,
};

int
main(int argc, char* argv[])
{
    size_t cycles = argc == 2 ? atoi(argv[1]) : -1;
    init_cp_precompute_buffer();
    engine = set_engine(node_8_cylinder);
    reset_engine(&engine);
    visualize_gamma();
    visualize_chamber_s();
    init_sdl();
    for(size_t cycle = 0; cycles == (size_t) -1 ? true : cycle < cycles; cycle++)
    {
        double flow_time_ms = 0.0;
        double mail_time_ms = 0.0;
        double move_time_ms = 0.0;
        double compress_time_ms = 0.0;
        double t0 = SDL_GetTicksNS();
        for(size_t i = 0; i < engine_cycles_per_frame; i++)
        {
            double ta = SDL_GetTicksNS();
            flow_engine(&engine);
            double tb = SDL_GetTicksNS();
            double tc = SDL_GetTicksNS();
            double td = SDL_GetTicksNS();
            move_engine(&engine);
            compress_engine_pistons(&engine);
            double te = SDL_GetTicksNS();
            flow_time_ms += tb - ta;
            mail_time_ms += tc - tb;
            move_time_ms += td - tc;
            compress_time_ms += te - td;
        }
        push_time_panel(&engine_time_panel, (float[]) {
            SDL_NS_TO_MS(flow_time_ms),
            SDL_NS_TO_MS(mail_time_ms),
            SDL_NS_TO_MS(move_time_ms),
            SDL_NS_TO_MS(compress_time_ms),
        });
        double t1 = SDL_GetTicksNS();
        if(handle_input(&engine))
        {
            break;
        }
        double t2 = SDL_GetTicksNS();
        clear_screen();
        draw_plots(&engine);
        draw_radial_chambers(&engine);
        draw_info(&engine, &loop_time_panel, &engine_time_panel, &rad_per_sec_progress_bar);
        double t3 = SDL_GetTicksNS();
        present(0.0);
        double t4 = SDL_GetTicksNS();
        push_time_panel(&loop_time_panel, (float[]) {
            SDL_NS_TO_MS(t1 - t0),
            SDL_NS_TO_MS(t2 - t1),
            SDL_NS_TO_MS(t3 - t2),
            SDL_NS_TO_MS(t4 - t0),
        });
    }
    exit_sdl();
}
