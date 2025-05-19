static constexpr size_t sdl_xres_p = 1800;
static constexpr size_t sdl_yres_p = 900;
static constexpr size_t sdl_mid_x_p = sdl_xres_p / 2;
static constexpr size_t sdl_mid_y_p = sdl_yres_p / 2;
static constexpr size_t sdl_node_w_p = 32;
static constexpr size_t sdl_node_half_w_p = sdl_node_w_p / 2;
static const char* sdl_title = "ensim4";
static SDL_Window* sdl_window = nullptr;
static SDL_Renderer* sdl_renderer = nullptr;
static constexpr SDL_FColor sdl_radial_zero_color = { .r = 0.0f, .g = 0.0f, .b = 0.0f, .a = 1.0f };
static constexpr SDL_FColor sdl_radial_node_color = { .r = 0.2f, .g = 0.2f, .b = 0.2f, .a = 1.0f };
static constexpr SDL_FColor sdl_radial_line_color = { .r = 0.4f, .g = 0.4f, .b = 0.4f, .a = 1.0f };
static constexpr SDL_FColor sdl_radial_name_color = { .r = 0.9f, .g = 0.0f, .b = 0.0f, .a = 1.0f };

static SDL_FPoint
add(SDL_FPoint self, const SDL_FPoint other)
{
    self.x += other.x;
    self.y += other.y;
    return self;
}

static void
set_render_color(const SDL_FColor color)
{
    SDL_SetRenderDrawColorFloat(sdl_renderer, color.r, color.g, color.b, color.a);
}

static void
init_sdl()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(sdl_title, sdl_xres_p, sdl_yres_p, 0, &sdl_window, &sdl_renderer);
}

static void
clear_screen()
{
    set_render_color(sdl_radial_zero_color);
    SDL_RenderClear(sdl_renderer);
}

static void
exit_sdl()
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
compute_radials(const struct engine_s* engine, SDL_FPoint points[])
{
    double radius_p = 3.0 * (engine->size * sdl_node_w_p) / (2.0 * std_pi_r);
    for(size_t i = 0; i < engine->size; i++)
    {
        double theta_r = 2.0 * std_pi_r * i / engine->size;
        points[i].x = sdl_mid_x_p + radius_p * cos(theta_r) - sdl_node_half_w_p;
        points[i].y = sdl_mid_y_p + radius_p * sin(theta_r) - sdl_node_half_w_p;
    }
}

static void
draw_radial_lines(const struct engine_s* engine, const SDL_FPoint points[])
{
    constexpr SDL_FPoint offset = { sdl_node_half_w_p, sdl_node_half_w_p };
    for(size_t i = 0; i < engine->size; i++)
    {
        SDL_FPoint a = add(points[i], offset);
        for(size_t next, j = 0; (next = engine->node[i].next[j]); j++)
        {
            SDL_FPoint b = add(points[next], offset);
            set_render_color(sdl_radial_line_color);
            SDL_RenderLine(sdl_renderer, a.x, a.y, b.x, b.y);
        }
    }
}

static void
draw_radial_nodes(const struct engine_s* engine, const SDL_FPoint points[])
{
    for(size_t i = 0; i < engine->size; i++)
    {
        SDL_FRect rect = {
            .x = points[i].x,
            .y = points[i].y,
            .w = sdl_node_w_p,
            .h = sdl_node_w_p,
        };
        set_render_color(sdl_radial_node_color);
        SDL_RenderFillRect(sdl_renderer, &rect);
        set_render_color(sdl_radial_line_color);
        SDL_RenderRect(sdl_renderer, &rect);
    }
}

static void
draw_radial_names(const struct engine_s* engine, const SDL_FPoint points[])
{
    for(size_t i = 0; i < engine->size; i++)
    {
        struct node_s* node = &engine->node[i];
        SDL_FPoint point = points[i];
        const char* name = node_name[node->type];
        size_t len = strlen(name);
        size_t half_char_size = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE / 2;
        double x_p = point.x + sdl_node_half_w_p - half_char_size * len;
        double y_p = point.y + sdl_node_half_w_p - half_char_size;
        set_render_color(sdl_radial_name_color);
        SDL_RenderDebugText(sdl_renderer, x_p, y_p, name);
    }
}

static void
draw_radial_chambers(const struct engine_s* engine)
{
    SDL_FPoint points[engine->size];
    compute_radials(engine, points);
    draw_radial_lines(engine, points);
    draw_radial_nodes(engine, points);
    draw_radial_names(engine, points);
}
