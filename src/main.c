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
#include "normalized_s.h"

/* isolated from engine core */
#include <SDL3/SDL.h>
#include "sdl_scroll_s.h"
#include "sdl_slide_buffer_t.h"
#include "sdl_time_panel_s.h"
#include "sdl_progress_bar_s.h"
#include "sdl.h"

static struct engine_s g_engine;

static struct sdl_time_panel_s g_loop_time_panel = {
    .title = "loop_time_ms",
    .labels = {
        "engine",
        "input",
        "draw",
        "vsync",
    },
    .min_time_ms = 0.0,
    .max_time_ms = 40.0,
    .rect.w = 192,
    .rect.h = 128,
};

static struct sdl_time_panel_s g_engine_time_panel = {
    .title = "engine_time_ms",
    .labels = {
        "fluids",
        "thermo",
        "TBD",
        "kinematics",
    },
    .min_time_ms = 0.0,
    .max_time_ms = 40.0,
    .rect.w = 192,
    .rect.h = 128,
};

static struct sdl_progress_bar_s g_rad_per_sec_progress_bar = {
    .title = "rad_per_sec",
    .max_value = 2000.0,
    .value = &g_engine.crankshaft.angular_velocity_r_per_s,
    .rect.w = 192,
    .rect.h = 16,
};

static double g_frames_per_sec = 0.0;

static struct sdl_progress_bar_s g_frames_per_sec_bar = {
    .title = "frames_per_sec",
    .max_value = 100.0,
    .value = &g_frames_per_sec,
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
    g_engine = set_engine(g_node_8_cylinder);
    reset_engine(&g_engine);
    init_sdl();
    for(size_t cycle = 0; cycles == (size_t) -1 ? true : cycle < cycles; cycle++)
    {
        double fluids_time_ms = 0.0;
        double thermo_time_ms = 0.0;
        double tbd_time_ms = 0.0;
        double kinematics_time_ms = 0.0;
        double t0 = SDL_GetTicks();
        for(size_t i = 0; i < g_engine_cycles_per_frame; i++)
        {
            double ta = SDL_GetTicks();
            flow_engine(&g_engine);
            double tb = SDL_GetTicks();
            double tc = SDL_GetTicks();
            double td = SDL_GetTicks();
            move_engine(&g_engine);
            compress_engine_pistons(&g_engine);
            double te = SDL_GetTicks();
            fluids_time_ms += tb - ta;
            thermo_time_ms += tc - tb;
            tbd_time_ms += td - tc;
            kinematics_time_ms += te - td;
        }
        push_time_panel(&g_engine_time_panel, (float[]) {
            fluids_time_ms,
            thermo_time_ms,
            tbd_time_ms,
            kinematics_time_ms,
        });
        double t1 = SDL_GetTicks();
        if(handle_input(&g_engine))
        {
            break;
        }
        double t2 = SDL_GetTicks();
        clear_screen();
        draw_plots(&g_engine);
        draw_radial_chambers(&g_engine);
        draw_info(
            &g_engine,
            &g_loop_time_panel,
            &g_engine_time_panel,
            &g_rad_per_sec_progress_bar,
            &g_frames_per_sec_bar);
        double t3 = SDL_GetTicks();
        present(0.0);
        double t4 = SDL_GetTicks();
        push_time_panel(&g_loop_time_panel, (float[]) {
            t1 - t0,
            t2 - t1,
            t3 - t2,
            t4 - t0,
        });
        double t5 = SDL_GetTicks();
        g_frames_per_sec = 1000.0 / (t5 - t0);
    }
    exit_sdl();
}
