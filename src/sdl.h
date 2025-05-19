static constexpr double sdl_xres_p = 1800.0;
static constexpr double sdl_yres_p = 900.0;
static constexpr double sdl_mid_x_p = sdl_xres_p / 2.0;
static constexpr double sdl_mid_y_p = sdl_yres_p / 2.0;
static constexpr double sdl_node_w_p = 32.0;
static constexpr double sdl_node_half_w_p = sdl_node_w_p / 2.0;
static constexpr double sdl_char_size_p = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;
static constexpr double sdl_half_char_size_p = sdl_char_size_p / 2.0;
static constexpr double sdl_line_spacing_p = 1.5 * sdl_char_size_p;
static constexpr double sdl_radial_spacing = 2.5;
static constexpr char sdl_title[] = "ensim4";
static constexpr SDL_FColor sdl_zero_color = { .r = 0.0f, .g = 0.0f, .b = 0.0f, .a = 1.0f };
static constexpr SDL_FColor sdl_node_color = { .r = 0.2f, .g = 0.2f, .b = 0.2f, .a = 1.0f };
static constexpr SDL_FColor sdl_line_color = { .r = 0.4f, .g = 0.4f, .b = 0.4f, .a = 1.0f };
static constexpr SDL_FColor sdl_name_color = { .r = 0.9f, .g = 0.0f, .b = 0.0f, .a = 1.0f };

static SDL_Window* sdl_window = nullptr;
static SDL_Renderer* sdl_renderer = nullptr;

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
    SDL_SetRenderVSync(sdl_renderer, SDL_RENDERER_VSYNC_ADAPTIVE);
}

static void
clear_screen()
{
    set_render_color(sdl_zero_color);
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

static double
compute_radial_radius_p(const struct engine_s* engine)
{
    return sdl_radial_spacing * (engine->size * sdl_node_w_p) / (2.0 * std_pi_r);
}

static double
compute_radial_diameter_p(const struct engine_s* engine)
{
    return 2.0 * compute_radial_radius_p(engine);
}

static double
compute_plot_column_width_p(const struct engine_s* engine)
{
    return 0.8 * (sdl_xres_p - compute_radial_diameter_p(engine)) / 2.0;
}

static void
compute_radials(const struct engine_s* engine, SDL_FPoint points[])
{
    double radius_p = compute_radial_radius_p(engine);
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
    SDL_FPoint offset = { sdl_node_half_w_p, sdl_node_half_w_p };
    for(size_t i = 0; i < engine->size; i++)
    {
        SDL_FPoint a = add(points[i], offset);
        for(size_t next, j = 0; (next = engine->node[i].next[j]); j++)
        {
            SDL_FPoint b = add(points[next], offset);
            set_render_color(sdl_line_color);
            SDL_RenderLine(sdl_renderer, a.x, a.y, b.x, b.y);
        }
    }
}

static void
draw_radial_nodes(const struct engine_s* engine, const SDL_FPoint points[])
{
    for(size_t i = 0; i < engine->size; i++)
    {
        SDL_FRect rect = { points[i].x, points[i].y, sdl_node_w_p, sdl_node_w_p };
        set_render_color(sdl_node_color);
        SDL_RenderFillRect(sdl_renderer, &rect);
        set_render_color(sdl_line_color);
        SDL_RenderRect(sdl_renderer, &rect);
    }
}

static SDL_FPoint
center_text(SDL_FPoint point, const char* text)
{
    size_t len = strlen(text);
    double x_p = point.x + sdl_node_half_w_p - sdl_half_char_size_p * len;
    double y_p = point.y + sdl_node_half_w_p - sdl_half_char_size_p;
    return (SDL_FPoint) { x_p, y_p };
}

static void
draw_radial_names(const struct engine_s* engine, const SDL_FPoint points[])
{
    for(size_t i = 0; i < engine->size; i++)
    {
        struct node_s* node = &engine->node[i];
        const char* name = node_name[node->type];
        SDL_FPoint point = center_text(points[i], name);
        set_render_color(sdl_name_color);
        SDL_RenderDebugText(sdl_renderer, point.x, point.y, name);
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

static void
draw_plot_columm(double x_p, double w_p, double h_p, size_t* index)
{
    double y_p = 0.0;
    while(y_p < sdl_yres_p)
    {
        set_render_color(sdl_line_color);
        SDL_FRect rect = { x_p, y_p, w_p, h_p };
        SDL_RenderRect(sdl_renderer, &rect);
        set_render_color(sdl_name_color);
        SDL_RenderDebugText(sdl_renderer, x_p + sdl_line_spacing_p, y_p + sdl_line_spacing_p, sample_name[*index]);
        y_p += h_p;
        *index += 1;
    }
}

static void
draw_plots(const struct engine_s* engine)
{
    size_t index = 0;
    double w_p = compute_plot_column_width_p(engine);
    double samples = len(sample_name);
    double left_samples = samples / 2.0;
    double left_h = sdl_yres_p / left_samples;
    double right_samples = samples - left_samples;
    double right_h = sdl_yres_p / right_samples;
    draw_plot_columm(0.0, w_p, left_h, &index);
    draw_plot_columm(sdl_xres_p - w_p, w_p, right_h, &index);
}

static void
draw_fps(const struct engine_s* engine, double sim_time_ms, double input_time_ms, double draw_time_ms, double vsync_time_ms)
{
    set_render_color(sdl_name_color);
    double x_p = compute_plot_column_width_p(engine) + sdl_line_spacing_p;
    double y_p = sdl_line_spacing_p;
    SDL_RenderDebugText(sdl_renderer, x_p, y_p, "ensim4");
    SDL_RenderDebugTextFormat(sdl_renderer, x_p, y_p += sdl_line_spacing_p, "sim   (ms) %6.3f", sim_time_ms);
    SDL_RenderDebugTextFormat(sdl_renderer, x_p, y_p += sdl_line_spacing_p, "input (ms) %6.3f", input_time_ms);
    SDL_RenderDebugTextFormat(sdl_renderer, x_p, y_p += sdl_line_spacing_p, "draw  (ms) %6.3f", draw_time_ms);
    SDL_RenderDebugTextFormat(sdl_renderer, x_p, y_p += sdl_line_spacing_p, "total (ms) %6.3f", sim_time_ms + input_time_ms + draw_time_ms);
    SDL_RenderDebugTextFormat(sdl_renderer, x_p, y_p += sdl_line_spacing_p, "vsync (ms) %6.3f", vsync_time_ms);
}
