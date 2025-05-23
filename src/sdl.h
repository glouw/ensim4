static constexpr char sdl_title[] = "ENSIM4";
static constexpr double sdl_xres_p = 1800.0;
static constexpr double sdl_yres_p = 900.0;
static constexpr double sdl_mid_x_p = sdl_xres_p / 2.0;
static constexpr double sdl_mid_y_p = sdl_yres_p / 2.0;
static constexpr double sdl_node_w_p = 24.0;
static constexpr double sdl_radial_spacing = 2.5;
static constexpr double sdl_node_half_w_p = sdl_node_w_p / 2.0;
static constexpr double sdl_char_size_p = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;
static constexpr double sdl_half_char_size_p = sdl_char_size_p / 2.0;
static constexpr double sdl_line_spacing_p = 1.5 * sdl_char_size_p;
static constexpr double sdl_demo_delay_ms = 75.0;
static constexpr double sdl_column_width_ratio = 0.5;
static constexpr size_t sdl_slide_buffer_size = 128;
static constexpr SDL_FColor sdl_channel_color[] = {
    [0]  = {1.00f, 0.00f, 0.00f, 1.0f},
    [1]  = {0.00f, 1.00f, 0.00f, 1.0f},
    [2]  = {0.00f, 0.50f, 1.00f, 1.0f},
    [3]  = {1.00f, 1.00f, 0.00f, 1.0f},
    [4]  = {1.00f, 0.00f, 1.00f, 1.0f},
    [5]  = {0.00f, 1.00f, 1.00f, 1.0f},
    [6]  = {1.00f, 0.65f, 0.00f, 1.0f},
    [7]  = {0.60f, 0.00f, 1.00f, 1.0f},
    [8]  = {0.00f, 1.00f, 0.60f, 1.0f},
    [9]  = {1.00f, 0.40f, 0.70f, 1.0f},
    [10] = {0.30f, 0.80f, 1.00f, 1.0f},
    [11] = {0.60f, 1.00f, 0.20f, 1.0f},
    [12] = {1.00f, 0.85f, 0.00f, 1.0f},
    [13] = {0.85f, 0.00f, 0.85f, 1.0f},
    [14] = {0.00f, 0.75f, 1.00f, 1.0f},
    [15] = {1.00f, 0.30f, 0.30f, 1.0f},
};
static_assert(sample_channels <= len(sdl_channel_color));
static constexpr SDL_FColor sdl_black_color = {
    0.00f, 0.00f, 0.00f, 0.0f
};
static constexpr SDL_FColor sdl_text_color = {
    1.00f, 1.00f, 1.00f, 1.0f
};
static constexpr SDL_FColor sdl_line_color = {
    0.50f, 0.50f, 0.50f, 1.0f
};
static constexpr SDL_FColor sdl_light_line_color = {
    0.25f, 0.25f, 0.25f, 1.0f
};
static constexpr SDL_FColor sdl_node_on_color = {
    0.00f, 1.00f, 0.00f, 1.0f
};
static constexpr SDL_FColor sdl_node_off_color = {
    1.00f, 0.00f, 0.00f, 1.0f
};
static constexpr SDL_FColor sdl_error_color = {
    1.00f, 0.00f, 0.00f, 1.0f
};

static SDL_Window* sdl_window = nullptr;
static SDL_Renderer* sdl_renderer = nullptr;

typedef double sdl_slide_buffer_t[sdl_slide_buffer_size];

static void
push_slide_buffer(sdl_slide_buffer_t self, double value)
{
    for(size_t i = 0; i < sdl_slide_buffer_size - 1; i++)
    {
        self[i] = self[i + 1];
    }
    self[sdl_slide_buffer_size - 1] = value;
}

static void
set_render_color(SDL_FColor color)
{
    SDL_SetRenderDrawColorFloat(sdl_renderer, color.r, color.g, color.b, color.a);
}

static void
draw_slide_buffer(sdl_slide_buffer_t self, SDL_FRect rect, SDL_FColor color, double min_val, double max_val)
{
    set_render_color(color);
    SDL_FPoint points[sdl_slide_buffer_size];
    double range = max_val - min_val;
    for (size_t i = 0; i < sdl_slide_buffer_size; i++)
    {
        points[i].x = rect.x + (rect.w * i) / (sdl_slide_buffer_size - 1);
        double normalized_val = (self[i] - min_val) / range;
        points[i].y = rect.y + rect.h * (1.0 - normalized_val);
    }
    SDL_RenderLines(sdl_renderer, points, sdl_slide_buffer_size);
}

struct sdl_time_panel_s
{
    SDL_FRect rect;
    double min_time_ms;
    double max_time_ms;
    sdl_slide_buffer_t sim;
    sdl_slide_buffer_t input;
    sdl_slide_buffer_t draw;
    sdl_slide_buffer_t vsync;
};

static void
draw_time_panel(struct sdl_time_panel_s* self)
{
    set_render_color(sdl_light_line_color);
    SDL_RenderRect(sdl_renderer, &self->rect);
    draw_slide_buffer(self->sim, self->rect, sdl_channel_color[0], self->min_time_ms, self->max_time_ms);
    draw_slide_buffer(self->input, self->rect, sdl_channel_color[1], self->min_time_ms, self->max_time_ms);
    draw_slide_buffer(self->draw, self->rect, sdl_channel_color[2], self->min_time_ms, self->max_time_ms);
    draw_slide_buffer(self->vsync, self->rect, sdl_channel_color[3], self->min_time_ms, self->max_time_ms);
}

struct sdl_time_panel_sample_s
{
    double sim_time_ms;
    double input_time_ms;
    double draw_time_ms;
    double vsync_time_ms;
};

static void
push_time_panel(struct sdl_time_panel_s* self, struct sdl_time_panel_sample_s sample)
{
    push_slide_buffer(self->sim, sample.sim_time_ms);
    push_slide_buffer(self->input, sample.input_time_ms);
    push_slide_buffer(self->draw, sample.draw_time_ms);
    push_slide_buffer(self->vsync, sample.vsync_time_ms);
}

static SDL_FPoint
add(SDL_FPoint self, SDL_FPoint other)
{
    self.x += other.x;
    self.y += other.y;
    return self;
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
    set_render_color(sdl_black_color);
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
present(double delay_ms)
{
    SDL_RenderPresent(sdl_renderer);
    if(delay_ms > 0.0)
    {
        SDL_Delay(delay_ms);
    }
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
    return sdl_column_width_ratio * (sdl_xres_p - compute_radial_diameter_p(engine)) / 2.0;
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

static SDL_FPoint
center_text(SDL_FPoint point, const char* text)
{
    size_t len = strlen(text);
    double x_p = point.x + sdl_node_half_w_p - sdl_half_char_size_p * len;
    double y_p = point.y + sdl_node_half_w_p - sdl_half_char_size_p;
    return (SDL_FPoint) { x_p, y_p };
}

static void
draw_node_at(struct node_s* node, SDL_FPoint point)
{
    SDL_FRect rect = { point.x, point.y, sdl_node_w_p, sdl_node_w_p };
    set_render_color(sdl_black_color);
    SDL_RenderFillRect(sdl_renderer, &rect);
    set_render_color(node->is_selected ? sdl_node_on_color : sdl_node_off_color);
    SDL_RenderRect(sdl_renderer, &rect);
    static const char* spinner[] = {"|", "/", "-", "\\", "|", "/", "-", "\\"};
    size_t cycle = node->as.chamber.flow_cycles / 1024;
    size_t index = cycle % len(spinner);
    const char* spin = spinner[index];
    SDL_FPoint mid = point;
    mid = center_text(mid, spin);
    SDL_RenderDebugText(sdl_renderer, mid.x, mid.y, spin);
}

static void
draw_line_by(SDL_FPoint a, SDL_FPoint b)
{
    struct SDL_FPoint shift = { sdl_node_half_w_p, sdl_node_half_w_p};
    a = add(a, shift);
    b = add(b, shift);
    set_render_color(sdl_light_line_color);
    SDL_RenderLine(sdl_renderer, a.x, a.y, b.x, b.y);
}

static void
draw_radial_lines(const struct engine_s* engine, const SDL_FPoint points[])
{
    for(size_t i = 0; i < engine->size; i++)
    {
        for(size_t next, j = 0; (next = engine->node[i].next[j]); j++)
        {
            draw_line_by(points[i], points[next]);
        }
    }
}

static void
draw_radial_nodes(const struct engine_s* engine, const SDL_FPoint points[])
{
    for(size_t i = 0; i < engine->size; i++)
    {
        struct node_s* node = &engine->node[i];
        draw_node_at(node, points[i]);
    }
}

static void
draw_radial_names(const struct engine_s* engine, const SDL_FPoint points[])
{
    for(size_t i = 0; i < engine->size; i++)
    {
        struct node_s* node = &engine->node[i];
        const char* name = node_name[node->type];
        SDL_FPoint point = points[i];
        point = center_text(point, name);
        point.y -= sdl_line_spacing_p;
        point.y -= sdl_node_half_w_p;
        set_render_color(sdl_text_color);
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
draw_plot_channel(SDL_FRect rects[], size_t channel)
{
    set_render_color(sdl_channel_color[channel]);
    static SDL_FPoint points[sample_e_size * sample_samples];
    static double samples[sample_samples];
    size_t runner = 0;
    for(enum sample_e index = 0; index < sample_e_size; index++)
    {
        double max_value = DBL_MIN;
        double min_value = DBL_MAX;
        for(size_t i = 0; i < sample_front_size; i++)
        {
            double sample = sample_front[channel][index][i];
            max_value = max(max_value, sample);
            min_value = min(min_value, sample);
            samples[i] = sample;
        }
        double range = max_value - min_value;
        if(range < 1e-9)
        {
            continue;
        }
        for(size_t i = 0; i < sample_front_size; i++)
        {
            samples[i] = (samples[i] - min_value) / range;
        }
        for(size_t i = 0; i < sample_front_size; i++)
        {
            points[runner].x = rects[index].x + (i / (double) (sample_front_size - 1)) * (rects[index].w - 1);
            points[runner].y = rects[index].y + (1.0 - samples[i]) * rects[index].h;
            runner += 1;
        }
    }
    SDL_RenderPoints(sdl_renderer, points, runner);
}

static void
draw_plot_channels(SDL_FRect rects[])
{
    for(size_t channel = 0; channel < sample_channels; channel++)
    {
        draw_plot_channel(rects, channel);
    }
}

static void
draw_plot_container(SDL_FRect rect, const char* name)
{
    set_render_color(sdl_line_color);
    SDL_RenderRect(sdl_renderer, &rect);
    set_render_color(sdl_text_color);
    SDL_RenderDebugText(
        sdl_renderer,
        rect.x + sdl_line_spacing_p,
        rect.y + sdl_line_spacing_p,
        name);
}

static void
draw_plot_containers(const SDL_FRect rects[])
{
    for(enum sample_e index = 0; index < sample_e_size; index++)
    {
        draw_plot_container(rects[index], sample_name[index]);
    }
}

static void
position_plot(double x_p, double w_p, double h_p, enum sample_e* index, SDL_FRect rects[])
{
    double y_p = 0.0;
    while(y_p < sdl_yres_p)
    {
        rects[*index] = (SDL_FRect) { x_p, y_p, w_p, h_p };
        y_p += h_p;
        *index += 1;
    }
}

static void
position_plots(const struct engine_s* engine, SDL_FRect rects[])
{
    enum sample_e index = 0;
    double w_p = compute_plot_column_width_p(engine);
    double samples = len(sample_name);
    double left_samples = round(samples / 2.0);
    double left_h = sdl_yres_p / left_samples;
    double right_samples = samples - left_samples;
    double right_h = sdl_yres_p / right_samples;
    position_plot(0.0, w_p, left_h, &index, rects);
    position_plot(sdl_xres_p - w_p, w_p, right_h, &index, rects);
}

static void
draw_plots(const struct engine_s* engine)
{
    SDL_FRect rects[sample_e_size];
    position_plots(engine, rects);
    draw_plot_channels(rects);
    draw_plot_containers(rects);
}

static void
draw_fps(const struct engine_s* engine, struct sdl_time_panel_s* time_panel, struct sdl_time_panel_sample_s sample)
{
    set_render_color(sdl_text_color);
    double x_p = compute_plot_column_width_p(engine) + sdl_line_spacing_p;
    double y_p = sdl_line_spacing_p;
    SDL_RenderDebugText(sdl_renderer, x_p, y_p, sdl_title);
    SDL_RenderDebugTextFormat(sdl_renderer, x_p, y_p += sdl_line_spacing_p, "vsync (ms): %6.3f", sample.vsync_time_ms);
    SDL_RenderDebugTextFormat(sdl_renderer, x_p, y_p += sdl_line_spacing_p, "sim   (ms): %6.3f", sample.sim_time_ms);
    SDL_RenderDebugTextFormat(sdl_renderer, x_p, y_p += sdl_line_spacing_p, "draw  (ms): %6.3f", sample.draw_time_ms);
    SDL_RenderDebugTextFormat(sdl_renderer, x_p, y_p += sdl_line_spacing_p, "input (ms): %6.3f", sample.input_time_ms);
    y_p += sdl_line_spacing_p;
    time_panel->rect.x = x_p;
    time_panel->rect.y = y_p;
    y_p += time_panel->rect.h;
    SDL_RenderDebugTextFormat(sdl_renderer, x_p, y_p += sdl_line_spacing_p, "engine size (bytes): %lu", engine->bytes);
    SDL_RenderDebugTextFormat(sdl_renderer, x_p, y_p += sdl_line_spacing_p, "trigger min (rad/sec): %.0f", sample_minimum_angular_velocity_r_per_s);
    SDL_RenderDebugTextFormat(sdl_renderer, x_p, y_p += sdl_line_spacing_p, "ang vel (rad/sec): %.0f", engine->angular_velocity_r_per_s);
}

static void
draw_demo_engine(const struct engine_s* engine)
{
    SDL_FPoint points[engine->size];
    compute_radials(engine, points);
    clear_screen();
    present(sdl_demo_delay_ms);
    for(size_t i = 0; i < engine->size; i++)
    {
        for(size_t next, j = 0; (next = engine->node[i].next[j]); j++)
        {
            draw_line_by(points[i], points[next]);
            struct node_s* x = &engine->node[i];
            struct node_s* y = &engine->node[j];
            draw_node_at(x, points[i]);
            draw_node_at(y, points[j]);
            present(sdl_demo_delay_ms);
        }
    }
}

static bool
handle_input(struct engine_s* engine)
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type)
    {
    case SDL_EVENT_QUIT:
        return true;
    case SDL_EVENT_KEY_UP:
        switch(event.key.key)
        {
        case SDLK_F:
            draw_demo_engine(engine);
            break;
        case SDLK_1:
            engine->angular_velocity_r_per_s = 20.0;
            break;
        case SDLK_2:
            engine->angular_velocity_r_per_s = 69.0;
            break;
        case SDLK_3:
            engine->angular_velocity_r_per_s = 100.0;
            break;
        case SDLK_4:
            engine->angular_velocity_r_per_s = 500.0;
            break;
        case SDLK_5:
            engine->angular_velocity_r_per_s = 1000.0;
            break;
        case SDLK_6:
            engine->angular_velocity_r_per_s = 1050.0;
            break;
        }
        break;
    }
    return false;
}
