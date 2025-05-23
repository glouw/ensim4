#include <SDL3/SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <float.h>
#include <math.h>
#include "std.h"
#include "gamma.h"
#include "gas_s.h"
#include "chamber_s.h"
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
#include "nozzle_flow_s.h"
#include "node_s.h"
#include "sample_s.h"
#include "engine_s.h"
#include "visualize.h"
#include "sdl.h"

int
main()
{
    struct engine_s engine = set_engine(node_three_cylinder);
    normalize_engine(&engine);
    engine.angular_velocity_r_per_s = 500.0;
    visualize_gamma();
    visualize_chamber_s();
    init_sdl();
    struct sdl_time_panel_sample_s time_panel_sample = {};
    struct sdl_time_panel_s time_panel = {
        .min_time_ms = 0.0,
        .max_time_ms = 32.0,
        .rect.w = 192,
        .rect.h = 128,
    };
    while(true)
    {
        double t0 = SDL_GetTicksNS();
        run_engine(&engine);
        double t1 = SDL_GetTicksNS();
        if(handle_input(&engine))
        {
            break;
        }
        double t2 = SDL_GetTicksNS();
        clear_screen();
        draw_plots(&engine);
        draw_radial_chambers(&engine);
        draw_fps(&engine, &time_panel, time_panel_sample);
        draw_time_panel(&time_panel);
        double t3 = SDL_GetTicksNS();
        present(0.0);
        double t4 = SDL_GetTicksNS();
        time_panel_sample.sim_time_ms = SDL_NS_TO_MS(t1 - t0);
        time_panel_sample.input_time_ms = SDL_NS_TO_MS(t2 - t1);
        time_panel_sample.draw_time_ms = SDL_NS_TO_MS(t3 - t2);
        time_panel_sample.vsync_time_ms = SDL_NS_TO_MS(t4 - t0);
        push_time_panel(&time_panel, time_panel_sample);
    }
    exit_sdl();
}
