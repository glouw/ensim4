#include <SDL3/SDL.h>
#include <stdio.h>
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
#include "engine_s.h"
#include "sample_s.h"
#include "log_t.h"
#include "visualize.h"
#include "sdl.h"

int
main()
{
    printf("%lu\n", sizeof(node_three_cylinder));
    printf("%lu\n", sizeof(log_log));
    struct engine_s engine = set_engine(node_three_cylinder);
    normalize_engine(&engine);
    visualize_gamma();
    visualize_chamber_s();
    init_sdl();
    double sim_time_ms = 0.0;
    double input_time_ms = 0.0;
    double draw_time_ms = 0.0;
    double vsync_time_ms = 0.0;
    while(true)
    {
        double t0 = SDL_GetTicksNS();
        flow_engine_by(&engine, 512);
        double t1 = SDL_GetTicksNS();
        SDL_Event event;
        SDL_PollEvent(&event);
        if(event.type == SDL_EVENT_QUIT)
        {
            break;
        }
        double t2 = SDL_GetTicksNS();
        clear_screen();
        draw_plots(&engine);
        draw_radial_chambers(&engine);
        draw_fps(&engine, sim_time_ms, input_time_ms, draw_time_ms, vsync_time_ms);
        double t3 = SDL_GetTicksNS();
        present();
        double t4 = SDL_GetTicksNS();
        sim_time_ms = SDL_NS_TO_MS(t1 - t0);
        input_time_ms = SDL_NS_TO_MS(t2 - t1);
        draw_time_ms = SDL_NS_TO_MS(t3 - t2);
        vsync_time_ms = SDL_NS_TO_MS(t4 - t3);
    }
    exit_sdl();
}
