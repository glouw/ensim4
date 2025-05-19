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
#include "visualize.h"
#include "sdl3.h"

int
main()
{
    printf("%lu\n", sizeof(three_cylinder));
    struct engine_s engine = set_engine(three_cylinder);
    normalize_engine(&engine);
    visualize_gamma();
    visualize_chamber_s();
    init_sdl3();
    while(true)
    {
        SDL_Event event;
        SDL_PollEvent(&event);
        if(event.type == SDL_EVENT_QUIT)
        {
            break;
        }
        clear_screen();
        draw_radial_chambers(&engine);
        present();
    }
    exit_sdl3();
}
