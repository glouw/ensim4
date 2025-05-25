static constexpr char sdl_title[] = "ENSIM4";
static constexpr float sdl_xres_p = 1800.0f;
static constexpr float sdl_yres_p = 900.0f;
static constexpr float sdl_mid_x_p = sdl_xres_p / 2.0f;
static constexpr float sdl_mid_y_p = sdl_yres_p / 2.0f;
static constexpr float sdl_node_w_p = 32.0f;
static constexpr float sdl_radial_spacing = 2.5f;
static constexpr float sdl_node_half_w_p = sdl_node_w_p / 2.0f;
static constexpr float sdl_char_size_p = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;
static constexpr float sdl_half_char_size_p = sdl_char_size_p / 2.0f;
static constexpr float sdl_line_spacing_p = 1.5f * sdl_char_size_p;
static constexpr float sdl_demo_delay_ms = 75.0f;
static constexpr float sdl_column_width_ratio = 0.5f;
static constexpr size_t sdl_slide_buffer_size = 128;
static constexpr size_t sdl_time_panel_size = 4;
static constexpr SDL_FColor sdl_channel_color[] = {
     [0] = {1.00f, 0.00f, 0.00f, 1.0f},
     [1] = {0.00f, 1.00f, 0.00f, 1.0f},
     [2] = {0.00f, 0.50f, 1.00f, 1.0f},
     [3] = {1.00f, 1.00f, 0.00f, 1.0f},
     [4] = {1.00f, 0.00f, 1.00f, 1.0f},
     [5] = {0.00f, 1.00f, 1.00f, 1.0f},
     [6] = {1.00f, 0.65f, 0.00f, 1.0f},
     [7] = {0.60f, 0.00f, 1.00f, 1.0f},
     [8] = {0.00f, 1.00f, 0.60f, 1.0f},
     [9] = {1.00f, 0.40f, 0.70f, 1.0f},
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
static const char* sdl_spinner[] = {
    "|",
    "/",
    "-",
    "\\",
    "|",
    "/",
    "-",
    "\\"
};

#define debugf SDL_RenderDebugTextFormat

static SDL_Window* sdl_window = nullptr;
static SDL_Renderer* sdl_renderer = nullptr;

struct sdl_scroll_s
{
    float x_p;
    float y_p;
};

static float
scroll_by(struct sdl_scroll_s* self, float dy_p)
{
    float y_p = self->y_p;
    self->y_p += dy_p;
    return y_p;
}

static float
newline(struct sdl_scroll_s* self)
{
    return scroll_by(self, sdl_line_spacing_p);
}

typedef float sdl_slide_buffer_t[sdl_slide_buffer_size];

static void
push_slide_buffer(sdl_slide_buffer_t self, float value)
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
draw_slide_buffer(sdl_slide_buffer_t self, SDL_FRect rect, float min_val, float max_val)
{
    SDL_FPoint points[sdl_slide_buffer_size];
    float range = max_val - min_val;
    for (size_t i = 0; i < sdl_slide_buffer_size; i++)
    {
        points[i].x = rect.x + (rect.w * i) / (sdl_slide_buffer_size - 1);
        float normalized_val = (self[i] - min_val) / range;
        points[i].y = rect.y + rect.h * (1.0f - normalized_val);
    }
    SDL_RenderLines(sdl_renderer, points, sdl_slide_buffer_size);
}

struct sdl_time_panel_s
{
    const char* title;
    const char* labels[sdl_time_panel_size];
    SDL_FRect rect;
    float min_time_ms;
    float max_time_ms;
    sdl_slide_buffer_t slide_buffer[sdl_time_panel_size];
};

static void
draw_time_panel(struct sdl_time_panel_s* self)
{
    set_render_color(sdl_light_line_color);
    SDL_RenderRect(sdl_renderer, &self->rect);
    for(size_t i = 0; i < sdl_time_panel_size; i++)
    {
        set_render_color(sdl_channel_color[i]);
        draw_slide_buffer(
            self->slide_buffer[i],
            self->rect,
            self->min_time_ms,
            self->max_time_ms);
    }
}

static void
push_time_panel(struct sdl_time_panel_s* self, float sample[])
{
    for(size_t i = 0; i < sdl_time_panel_size; i++)
    {
        push_slide_buffer(self->slide_buffer[i], sample[i]);
    }
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
present(float delay_ms)
{
    SDL_RenderPresent(sdl_renderer);
    if(delay_ms > 0.0f)
    {
        SDL_Delay(delay_ms);
    }
}

static float
compute_radial_radius_p(const struct engine_s* engine)
{
    return sdl_radial_spacing * (engine->size * sdl_node_w_p) / (2.0f * std_pi_r);
}

static float
compute_radial_diameter_p(const struct engine_s* engine)
{
    return 2.0f * compute_radial_radius_p(engine);
}

static float
compute_plot_column_width_p(const struct engine_s* engine)
{
    return sdl_column_width_ratio * (sdl_xres_p - compute_radial_diameter_p(engine)) / 2.0f;
}

static void
compute_radials(const struct engine_s* engine, SDL_FPoint points[])
{
    float radius_p = compute_radial_radius_p(engine);
    for(size_t i = 0; i < engine->size; i++)
    {
        float theta_r = 2.0f * std_pi_r * i / engine->size;
        points[i].x = sdl_mid_x_p + radius_p * cos(theta_r) - sdl_node_half_w_p;
        points[i].y = sdl_mid_y_p + radius_p * sin(theta_r) - sdl_node_half_w_p;
    }
}

static SDL_FPoint
center_text(SDL_FPoint point, const char* text)
{
    size_t len = strlen(text);
    float x_p = point.x + sdl_node_half_w_p - sdl_half_char_size_p * len;
    float y_p = point.y + sdl_node_half_w_p - sdl_half_char_size_p;
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
    size_t cycle = node->as.chamber.flow_cycles / 1024;
    size_t index = cycle % len(sdl_spinner);
    const char* spinner = sdl_spinner[index];
    SDL_FPoint mid = point;
    mid = center_text(mid, spinner);
    debugf(sdl_renderer, mid.x, mid.y, "%s", spinner);
}

static void
draw_line_by(SDL_FPoint a, SDL_FPoint b)
{
    struct SDL_FPoint shift = {
        sdl_node_half_w_p,
        sdl_node_half_w_p,
    };
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
        const char* node_name = node_name_string[node->type];
        SDL_FPoint point = points[i];
        point = center_text(point, node_name);
        point.y -= sdl_line_spacing_p;
        point.y -= sdl_node_half_w_p;
        set_render_color(sdl_text_color);
        debugf(sdl_renderer, point.x, point.y, "%s", node_name);
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

struct sdl_normalized_s
{
    float max_value;
    float min_value;
    bool is_success;
};

static struct sdl_normalized_s
normalize_samples(float samples[], size_t channel, enum sample_name_e sample_name)
{
    for(size_t i = 0; i < sample_size; i++)
    {
        samples[i] = sample_sample[channel][sample_name][i];
    }
    struct sdl_normalized_s normalized = {
        .max_value = FLT_MIN,
        .min_value = FLT_MAX,
        .is_success = false,
    };
    for(size_t i = 0; i < sample_size; i++)
    {
        normalized.max_value = max(normalized.max_value, samples[i]);
    }
    for(size_t i = 0; i < sample_size; i++)
    {
        normalized.min_value = min(normalized.min_value, samples[i]);
    }
    float range = normalized.max_value - normalized.min_value;
    if(range < 1e-9f)
    {
        return normalized;
    }
    for(size_t i = 0; i < sample_size; i++)
    {
        samples[i] = (samples[i] - normalized.min_value) / range;
    }
    normalized.is_success = true;
    return normalized;
}

static void
buffer_samples(SDL_FPoint buffer[], size_t* buffered, float samples[], SDL_FRect rect)
{
    for(size_t i = 0; i < sample_size; i++)
    {
        SDL_FPoint point = {
            rect.x + (i / (sample_size - 1.0f)) * (rect.w - 1.0f),
            rect.y + (1.0f - samples[i]) * rect.h,
        };
        buffer[(*buffered)++] = point;
    }
}

static void
draw_plot_channel(SDL_FRect rects[], size_t channel)
{
    size_t buffered = 0;
    static SDL_FPoint buffer[sample_name_e_size * sample_samples];
    static float samples[sample_samples];
    for(enum sample_name_e sample_name = 0; sample_name < sample_name_e_size; sample_name++)
    {
        struct sdl_normalized_s normalized = normalize_samples(samples, channel, sample_name);
        SDL_FRect rect = rects[sample_name];
        struct sdl_scroll_s scroll = {
            rect.x + sdl_line_spacing_p,
            rect.y + rect.h / 2.0,
        };
        size_t last_channel = sample_channel_index - 1;
        if(channel == last_channel)
        {
            set_render_color(sdl_channel_color[last_channel]);
            debugf(sdl_renderer, scroll.x_p, newline(&scroll), "max: %+.3e", normalized.max_value);
            debugf(sdl_renderer, scroll.x_p, newline(&scroll), "min: %+.3e", normalized.min_value);
        }
        if(normalized.is_success == false)
        {
            continue;
        }
        buffer_samples(buffer, &buffered, samples, rect);
    }
    set_render_color(sdl_channel_color[channel]);
    SDL_RenderPoints(sdl_renderer, buffer, buffered);
}

static void
draw_plot_channels(SDL_FRect rects[])
{
    for(size_t channel = 0; channel < sample_channel_index; channel++)
    {
        draw_plot_channel(rects, channel);
    }
}

static void
draw_plot_container(SDL_FRect rect, const char* name_string)
{
    set_render_color(sdl_line_color);
    SDL_RenderRect(sdl_renderer, &rect);
    set_render_color(sdl_text_color);
    float x_p = rect.x + sdl_line_spacing_p;
    float y_p = rect.y + sdl_line_spacing_p;
    debugf(sdl_renderer, x_p, y_p, "%s", name_string);
}

static void
draw_plot_containers(const SDL_FRect rects[])
{
    for(enum sample_name_e sample_name = 0; sample_name < sample_name_e_size; sample_name++)
    {
        const SDL_FRect rect = rects[sample_name];
        const char* name_string = skip_sample_namespace(sample_name_string[sample_name]);
        draw_plot_container(rect, name_string);
    }
}

static void
position_plot(float x_p, float w_p, float h_p, enum sample_name_e* sample_name, SDL_FRect rects[])
{
    float y_p = 0.0;
    while(y_p < sdl_yres_p)
    {
        rects[(*sample_name)++] = (SDL_FRect) { x_p, y_p, w_p, h_p };
        y_p += h_p;
    }
}

static void
position_plots(const struct engine_s* engine, SDL_FRect rects[])
{
    enum sample_name_e sample_name = 0;
    float w_p = compute_plot_column_width_p(engine);
    float left_samples = roundf(sample_name_e_size/ 2.0f);
    float right_samples = sample_name_e_size - left_samples;
    float left_h = ceilf(sdl_yres_p / left_samples);
    float right_h = ceilf(sdl_yres_p / right_samples);
    position_plot(0.0f, w_p, left_h, &sample_name, rects);
    position_plot(sdl_xres_p - w_p, w_p, right_h, &sample_name, rects);
}

static void
draw_plots(const struct engine_s* engine)
{
    SDL_FRect rects[sample_name_e_size];
    position_plots(engine, rects);
    draw_plot_channels(rects);
    draw_plot_containers(rects);
}

static void
draw_time_panel_info(struct sdl_time_panel_s* time_panel, struct sdl_scroll_s* scroll)
{
    set_render_color(sdl_text_color);
    debugf(sdl_renderer, scroll->x_p, newline(scroll), "%s", time_panel->title);
    for(size_t i = 0; i < sdl_time_panel_size; i++)
    {
        set_render_color(sdl_channel_color[i]);
        float sample = time_panel->slide_buffer[i][sdl_slide_buffer_size - 1];
        debugf(sdl_renderer, scroll->x_p, newline(scroll), "%8s: %6.3f", time_panel->labels[i], sample);
    }
    time_panel->rect.x = scroll->x_p;
    time_panel->rect.y = scroll->y_p;
    draw_time_panel(time_panel);
    newline(scroll);
    scroll_by(scroll, time_panel->rect.h);
}

static void
draw_engine_info(const struct engine_s* engine, struct sdl_scroll_s* scroll)
{
    set_render_color(sdl_text_color);
    debugf(sdl_renderer, scroll->x_p, newline(scroll), "engine_size_bytes: %lu", engine->bytes);
    debugf(sdl_renderer, scroll->x_p, newline(scroll), "trigger_min_r_per_s: %.0f", sample_minimum_angular_velocity_r_per_s);
    debugf(sdl_renderer, scroll->x_p, newline(scroll), "ang_vel_r_per_s: %.0f", engine->crankshaft.angular_velocity_r_per_s);
}

static void
draw_info_title(struct sdl_scroll_s* scroll)
{
    set_render_color(sdl_text_color);
    debugf(sdl_renderer, scroll->x_p, newline(scroll), "%s", sdl_title);
}

static void
draw_info(const struct engine_s* engine, struct sdl_time_panel_s* loop_time_panel, struct sdl_time_panel_s* engine_time_panel)
{
    struct sdl_scroll_s scroll = {
        .x_p = compute_plot_column_width_p(engine) + sdl_line_spacing_p,
        .y_p = sdl_line_spacing_p,
    };
    draw_info_title(&scroll);
    draw_time_panel_info(loop_time_panel, &scroll);
    draw_time_panel_info(engine_time_panel, &scroll);
    draw_engine_info(engine, &scroll);
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

static void
toggle_node_at(struct engine_s* engine, float x_p, float y_p)
{
    SDL_FPoint points[engine->size];
    compute_radials(engine, points);
    for(size_t i = 0; i < engine->size; i++)
    {
        struct node_s* node = &engine->node[i];
        SDL_FPoint point = points[i];
        SDL_FRect rect = {
            point.x,
            point.y,
            sdl_node_w_p,
            sdl_node_w_p,
        };
        SDL_FPoint select = { x_p, y_p };
        if(SDL_PointInRectFloat(&select, &rect))
        {
            clear_samples();
            node->is_selected ^= true;
            break;
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
            engine->crankshaft.angular_velocity_r_per_s = 20.0;
            break;
        case SDLK_2:
            engine->crankshaft.angular_velocity_r_per_s = 69.0;
            break;
        case SDLK_3:
            engine->crankshaft.angular_velocity_r_per_s = 100.0;
            break;
        case SDLK_4:
            engine->crankshaft.angular_velocity_r_per_s = 500.0;
            break;
        case SDLK_5:
            engine->crankshaft.angular_velocity_r_per_s = 1000.0;
            break;
        case SDLK_6:
            engine->crankshaft.angular_velocity_r_per_s = 1050.0;
            break;
        case SDLK_P:
            deselect_all_nodes(engine->node, engine->size);
            select_nodes(engine->node, engine->size, is_piston);
            break;
        case SDLK_I:
            deselect_all_nodes(engine->node, engine->size);
            select_nodes(engine->node, engine->size, is_filter);
            select_nodes(engine->node, engine->size, is_throttle);
            select_nodes(engine->node, engine->size, is_iplenum);
            select_nodes(engine->node, engine->size, is_irunner);
            break;
        case SDLK_E:
            deselect_all_nodes(engine->node, engine->size);
            select_nodes(engine->node, engine->size, is_eplenum);
            select_nodes(engine->node, engine->size, is_erunner);
            select_nodes(engine->node, engine->size, is_exhaust);
            break;
        case SDLK_C:
            deselect_all_nodes(engine->node, engine->size);
            break;
        }
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        switch(event.button.button)
        {
        case SDL_BUTTON_LEFT:
            toggle_node_at(engine, event.button.x, event.button.y);
            break;
        case SDL_BUTTON_RIGHT:
            break;
        }
        break;
    }
    return false;
}
