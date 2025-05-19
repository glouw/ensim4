static constexpr size_t xres_p = 1800;
static constexpr size_t yres_p = 900;
static const char* title = "ensim4";
static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static constexpr double radial_w_p = 32.0;
static constexpr double radial_space_ratio = 4.0;

static void
init_sdl3()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(title, xres_p, yres_p, 0, &window, &renderer);
}

static void
clear_screen()
{
    SDL_SetRenderDrawColorFloat(renderer, 0.0, 0.0, 0.0, 1.0);
    SDL_RenderClear(renderer);
}

static void
exit_sdl3()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

static void
present()
{
    SDL_RenderPresent(renderer);
}

static void
draw_radial_chambers(struct engine_s* engine)
{
    double mid_x_p = xres_p / 2;
    double mid_y_p = yres_p / 2;
    double elements = engine->size;
    double radius_p = radial_space_ratio * (elements * radial_w_p) / (2.0 * pi_r);
    SDL_FPoint points[engine->size];
    for(size_t i = 0; i < elements; i++)
    {
        double theta_r = 2.0 * pi_r * i / engine->size;
        points[i].x = mid_x_p + radius_p * cos(theta_r) - radial_w_p / 2.0;
        points[i].y = mid_y_p + radius_p * sin(theta_r) - radial_w_p / 2.0;
    }

    /* draw connecting lines */
    for(size_t i = 0; i < elements; i++)
    {
        SDL_SetRenderDrawColorFloat(renderer, 0.5, 0.5, 0.5, 1.0);
        SDL_FPoint a = points[i];
        size_t j = 0;
        size_t next;
        while((next = engine->node[i].next[j++]))
        {
            SDL_FPoint b = points[next];
            SDL_RenderLine(
                renderer,
                a.x + radial_w_p / 2.0,
                a.y + radial_w_p / 2.0,
                b.x + radial_w_p / 2.0,
                b.y + radial_w_p / 2.0);
        }
    }

    /* draw node squares */
    for(size_t i = 0; i < elements; i++)
    {
        SDL_SetRenderDrawColorFloat(renderer, 1.0, 1.0, 1.0, 1.0);
        SDL_FRect rect = {
            .x = points[i].x,
            .y = points[i].y,
            .w = radial_w_p,
            .h = radial_w_p,
        };
        SDL_RenderFillRect(renderer, &rect);
    }

    /* draw labels */
    for(size_t i = 0; i < elements; i++)
    {
        struct node_s* node = &engine->node[i];
        SDL_SetRenderDrawColorFloat(renderer, 1.0, 0.0, 0.0, 1.0);
        SDL_FPoint point = points[i];
        size_t x_p = point.x;
        size_t y_p = point.y - 10;
        SDL_RenderDebugText(renderer, x_p, y_p, node_name[node->type]);
    }
}
