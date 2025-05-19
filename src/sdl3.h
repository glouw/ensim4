static constexpr size_t sdl_xres_p = 1800;
static constexpr size_t sdl_yres_p = 900;
static const char* sdl_title = "ensim4";
static SDL_Window* sdl_window = nullptr;
static SDL_Renderer* sdl_renderer = nullptr;

static void
init_sdl3()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(sdl_title, sdl_xres_p, sdl_yres_p, 0, &sdl_window, &sdl_renderer);
}

static void
clear_screen()
{
    SDL_SetRenderDrawColorFloat(sdl_renderer, 0.0, 0.0, 0.0, 1.0);
    SDL_RenderClear(sdl_renderer);
}

static void
exit_sdl3()
{
    SDL_DestroyWindow(sdl_window);
    SDL_DestroyRenderer(sdl_renderer);
    SDL_Quit();
}

static void
present()
{
    SDL_RenderPresent(sdl_renderer);
}

static void
draw_radial_chambers(struct engine_s* engine)
{
    double w_p = 32;
    double mid_x_p = sdl_xres_p / 2;
    double mid_y_p = sdl_yres_p / 2;
    double elements = engine->size;
    double radius_p = 3.0 * (elements * w_p) / (2.0 * std_pi_r);
    SDL_FPoint points[engine->size];
    for(size_t i = 0; i < elements; i++)
    {
        double theta_r = 2.0 * std_pi_r * i / engine->size;
        points[i].x = mid_x_p + radius_p * cos(theta_r) - w_p / 2.0;
        points[i].y = mid_y_p + radius_p * sin(theta_r) - w_p / 2.0;
    }

    /* draw connecting lines */
    for(size_t i = 0; i < elements; i++)
    {
        SDL_SetRenderDrawColorFloat(sdl_renderer, 0.5, 0.5, 0.5, 1.0);
        SDL_FPoint a = points[i];
        size_t j = 0;
        size_t next;
        while((next = engine->node[i].next[j++]))
        {
            SDL_FPoint b = points[next];
            SDL_RenderLine(
                sdl_renderer,
                a.x + w_p / 2.0,
                a.y + w_p / 2.0,
                b.x + w_p / 2.0,
                b.y + w_p / 2.0);
        }
    }

    /* draw node squares */
    for(size_t i = 0; i < elements; i++)
    {
        SDL_SetRenderDrawColorFloat(sdl_renderer, 0.25, 0.25, 0.25, 1.0);
        SDL_FRect rect = {
            .x = points[i].x,
            .y = points[i].y,
            .w = w_p,
            .h = w_p,
        };
        SDL_RenderFillRect(sdl_renderer, &rect);
        SDL_SetRenderDrawColorFloat(sdl_renderer, 0.5, 0.5, 0.5, 1.0);
        SDL_RenderRect(sdl_renderer, &rect);
    }

    /* draw labels */
    for(size_t i = 0; i < elements; i++)
    {
        struct node_s* node = &engine->node[i];
        SDL_SetRenderDrawColorFloat(sdl_renderer, 1.0, 1.0, 0.0, 1.0);
        SDL_FPoint point = points[i];
        const char* label = node_name[node->type];
        size_t len = strlen(label);
        double x_p = point.x + w_p / 2.0 - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * len / 2;
        double y_p = point.y + w_p / 2.0 - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE / 2;
        SDL_RenderDebugText(sdl_renderer, x_p, y_p, label);
    }
}
