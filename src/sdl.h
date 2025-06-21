#define debugf SDL_RenderDebugTextFormat

static const char* const g_sdl_title = "ENSIM4";
static constexpr float g_sdl_xres_p = 1880.0f;
static constexpr float g_sdl_yres_p = 1000.0f;
static constexpr float g_sdl_mid_x_p = g_sdl_xres_p / 2.0f;
static constexpr float g_sdl_mid_y_p = g_sdl_yres_p / 2.0f;
static constexpr float g_sdl_node_w_p = 32.0f;
static constexpr float g_sdl_radial_spacing = 2.5f;
static constexpr float g_sdl_node_half_w_p = g_sdl_node_w_p / 2.0f;
static constexpr float g_sdl_demo_delay_ms = 75.0f;
static constexpr float g_sdl_column_width_ratio = 0.55f;
static constexpr size_t g_sdl_flow_cycle_spinner_divisor = 2048;

static constexpr SDL_FColor g_sdl_channel_color[] = {
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

static SDL_FColor
get_channel_color(size_t index)
{
    return g_sdl_channel_color[index % len(g_sdl_channel_color)];
}

static constexpr SDL_FColor g_sdl_black_color = {
    0.00f, 0.00f, 0.00f, 0.0f
};

static constexpr SDL_FColor g_sdl_line_color = {
    0.25f, 0.25f, 0.25f, 1.0f
};

static constexpr SDL_FColor g_sdl_container_color = {
    0.50f, 0.50f, 0.50f, 1.0f
};

static constexpr SDL_FColor g_sdl_text_color = {
    1.00f, 1.00f, 1.00f, 1.0f
};

static const char* g_sdl_spinner[] = { "|", "/", "-", "\\", "|", "/", "-", "\\" };

static SDL_Window* g_sdl_window = nullptr;
static SDL_Renderer* g_sdl_renderer = nullptr;

static void
set_render_color(SDL_FColor color)
{
    SDL_SetRenderDrawColorFloat(g_sdl_renderer, color.r, color.g, color.b, color.a);
}

static void
draw_slide_buffer(const sdl_slide_buffer_t self, const SDL_FRect* rect, float min_val, float max_val)
{
    SDL_FPoint points[g_sdl_slide_buffer_size];
    float range = max_val - min_val;
    for (size_t i = 0; i < g_sdl_slide_buffer_size; i++)
    {
        float x = rect->x + 1;
        float y = rect->y + 1;
        float w = rect->w - 2;
        float h = rect->h - 2;
        points[i].x = x + (w * i) / (g_sdl_slide_buffer_size - 1);
        float normalized_val = (self[i] - min_val) / range;
        points[i].y = y + h * (1.0f - normalized_val);
    }
    SDL_RenderLines(g_sdl_renderer, points, g_sdl_slide_buffer_size);
}

static void
draw_time_panel(const struct sdl_time_panel_s* self)
{
    for(size_t i = 0; i < g_sdl_time_panel_size; i++)
    {
        const char* label = self->labels[i];
        if(label)
        {
            set_render_color(get_channel_color(i));
            draw_slide_buffer(self->slide_buffer[i], &self->rect, self->min_value, self->max_value);
        }
    }
    set_render_color(g_sdl_container_color);
    SDL_RenderRect(g_sdl_renderer, &self->rect);
}

static void
draw_progress_bar(struct sdl_progress_bar_s* self)
{
    double percent = (self->max_value > 0.0) ? (self->value / self->max_value) : 0.0;
    percent = clamp(percent, 0.0, 1.0);
    SDL_FRect fill = self->rect;
    fill.w = self->rect.w * percent;
    set_render_color(g_sdl_line_color);
    SDL_RenderFillRect(g_sdl_renderer, &fill);
    set_render_color(g_sdl_container_color);
    SDL_RenderRect(g_sdl_renderer, &self->rect);
}

static void
add_point(SDL_FPoint* self, const SDL_FPoint* other)
{
    self->x += other->x;
    self->y += other->y;
}

static void
init_sdl()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_CreateWindowAndRenderer(g_sdl_title, g_sdl_xres_p, g_sdl_yres_p, 0, &g_sdl_window, &g_sdl_renderer);
    SDL_SetRenderVSync(g_sdl_renderer, SDL_RENDERER_VSYNC_ADAPTIVE);
}

static void
clear_screen()
{
    set_render_color(g_sdl_black_color);
    SDL_RenderClear(g_sdl_renderer);
}

static void
exit_sdl()
{
    SDL_DestroyWindow(g_sdl_window);
    SDL_DestroyRenderer(g_sdl_renderer);
    SDL_Quit();
}

static void
present(float delay_ms)
{
    SDL_RenderPresent(g_sdl_renderer);
    if(delay_ms > 0.0f)
    {
        SDL_Delay(delay_ms);
    }
}

static float
compute_radial_radius_p(const struct engine_s* engine)
{
    return g_sdl_radial_spacing * (engine->size * g_sdl_node_w_p) / (2.0f * g_std_pi_r);
}

static float
compute_radial_diameter_p(const struct engine_s* engine)
{
    return 2.0f * compute_radial_radius_p(engine);
}

static float
compute_plot_column_width_p(const struct engine_s* engine)
{
    return g_sdl_column_width_ratio * (g_sdl_xres_p - compute_radial_diameter_p(engine)) / 2.0f;
}

static void
compute_radials(const struct engine_s* engine, SDL_FPoint points[])
{
    float radius_p = compute_radial_radius_p(engine);
    for(size_t i = 0; i < engine->size; i++)
    {
        float theta_r = 2.0f * g_std_pi_r * i / engine->size;
        points[i].x = g_sdl_mid_x_p + radius_p * cos(theta_r) - g_sdl_node_half_w_p;
        points[i].y = g_sdl_mid_y_p + radius_p * sin(theta_r) - g_sdl_node_half_w_p;
    }
}

static SDL_FPoint
center_text(SDL_FPoint point, const char* text)
{
    size_t len = strlen(text);
    float x_p = point.x + g_sdl_node_half_w_p - g_sdl_half_char_size_p * len;
    float y_p = point.y + g_sdl_node_half_w_p - g_sdl_half_char_size_p;
    return (SDL_FPoint) { x_p, y_p };
}

static void
draw_node_at(struct node_s* node, SDL_FPoint point, SDL_FColor node_color)
{
    SDL_FRect rect = { point.x, point.y, g_sdl_node_w_p, g_sdl_node_w_p };
    set_render_color(g_sdl_black_color);
    SDL_RenderFillRect(g_sdl_renderer, &rect);
    set_render_color(node_color);
    SDL_RenderRect(g_sdl_renderer, &rect);
    size_t cycle = node->as.chamber.flow_cycles / g_sdl_flow_cycle_spinner_divisor;
    size_t index = cycle % len(g_sdl_spinner);
    const char* spinner = g_sdl_spinner[index];
    SDL_FPoint mid = point;
    mid = center_text(mid, spinner);
    debugf(g_sdl_renderer, mid.x, mid.y, "%s", spinner);
}

static void
draw_line_by(SDL_FPoint from, SDL_FPoint to)
{
    struct SDL_FPoint shift = {
        g_sdl_node_half_w_p,
        g_sdl_node_half_w_p,
    };
    add_point(&from, &shift);
    add_point(&to, &shift);
    set_render_color(g_sdl_line_color);
    SDL_RenderLine(g_sdl_renderer, from.x, from.y, to.x, to.y);
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
    size_t color_index = 0;
    for(size_t i = 0; i < engine->size; i++)
    {
        struct node_s* node = &engine->node[i];
        SDL_FPoint point = points[i];
        if(node->is_selected)
        {
            SDL_FColor node_color = get_channel_color(color_index);
            draw_node_at(node, point, node_color);
            color_index += count_node_edges(node);
        }
        else
        {
            draw_node_at(node, point, g_sdl_container_color);
        }
    }
}

static void
draw_radial_names(const struct engine_s* engine, const SDL_FPoint points[])
{
    for(size_t i = 0; i < engine->size; i++)
    {
        struct node_s* node = &engine->node[i];
        const char* node_name = g_node_name_string[node->type];
        SDL_FPoint point = points[i];
        point = center_text(point, node_name);
        point.y -= g_sdl_line_spacing_p;
        point.y -= g_sdl_node_half_w_p;
        set_render_color(g_sdl_text_color);
        debugf(g_sdl_renderer, point.x, point.y, "%s", node_name);
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
draw_plot_channel(const SDL_FRect rects[], size_t channel, const struct sampler_s* sampler)
{
    size_t buffered = 0;
    static SDL_FPoint buffer[g_sample_name_e_size * g_sampler_max_samples];
    static float samples[g_sampler_max_samples];
    for(enum sample_name_e sample_name = 0; sample_name < g_sample_name_e_size; sample_name++)
    {
        for(size_t i = 0; i < sampler->size; i++)
        {
            samples[i] = sampler->value[channel][sample_name][i];
        }
        struct normalized_s normalized = normalize_samples(samples, sampler->size);
        const SDL_FRect* rect = &rects[sample_name];
        struct sdl_scroll_s scroll = {
            rect->x + 1 * g_sdl_line_spacing_p,
            rect->y + 2 * g_sdl_line_spacing_p,
        };
        if(channel == sampler->channel_index - 1)
        {
            set_render_color(get_channel_color(channel));
            debugf(g_sdl_renderer, scroll.x_p, newline(&scroll), "max: %+.3e", normalized.max_value);
            debugf(g_sdl_renderer, scroll.x_p, newline(&scroll), "min: %+.3e", normalized.min_value);
        }
        if(normalized.is_success == false)
        {
            continue;
        }
        for(size_t i = 0; i < sampler->size; i++)
        {
            buffer[buffered++] = (SDL_FPoint) {
                rect->x + (i / (sampler->size - 1.0f)) * (rect->w - 1.0f),
                rect->y + (1.0f - samples[i]) * rect->h,
            };
        }
    }
    set_render_color(get_channel_color(channel));
    SDL_RenderPoints(g_sdl_renderer, buffer, buffered);
}

static void
draw_plot_channels(const SDL_FRect rects[], const struct sampler_s* sampler)
{
    for(size_t channel = 0; channel < sampler->channel_index; channel++)
    {
        draw_plot_channel(rects, channel, sampler);
    }
}

static void
draw_plot_container(const SDL_FRect rect, const char* name_string)
{
    set_render_color(g_sdl_container_color);
    SDL_RenderRect(g_sdl_renderer, &rect);
    set_render_color(g_sdl_text_color);
    float x_p = rect.x + g_sdl_line_spacing_p;
    float y_p = rect.y + g_sdl_line_spacing_p;
    debugf(g_sdl_renderer, x_p, y_p, "%s", name_string);
}

static void
draw_plot_containers(const SDL_FRect rects[])
{
    for(enum sample_name_e sample_name = 0; sample_name < g_sample_name_e_size; sample_name++)
    {
        const SDL_FRect rect = rects[sample_name];
        const char* name_string = skip_sample_namespace(g_sample_name_string[sample_name]);
        draw_plot_container(rect, name_string);
    }
}

static void
position_plot_container(float x_p, float w_p, float h_p, enum sample_name_e* sample_name, SDL_FRect rects[])
{
    float y_p = 0.0;
    while(y_p < g_sdl_yres_p)
    {
        rects[(*sample_name)++] = (SDL_FRect) { x_p, y_p, w_p, h_p };
        y_p += h_p;
    }
}

static void
position_plot_containers(const struct engine_s* engine, SDL_FRect rects[])
{
    enum sample_name_e sample_name = 0;
    float w_p = compute_plot_column_width_p(engine);
    float left_samples = roundf(g_sample_name_e_size / 2.0f);
    float right_samples = g_sample_name_e_size - left_samples;
    float left_h = ceilf(g_sdl_yres_p / left_samples);
    float right_h = ceilf(g_sdl_yres_p / right_samples);
    position_plot_container(0.0f, w_p, left_h, &sample_name, rects);
    position_plot_container(g_sdl_xres_p - w_p, w_p, right_h, &sample_name, rects);
}

static void
draw_plots(const struct engine_s* engine, const struct sampler_s* sampler)
{
    static SDL_FRect rects[g_sample_name_e_size];
    position_plot_containers(engine, rects);
    draw_plot_channels(rects, sampler);
    draw_plot_containers(rects);
}

static void
draw_progress_bar_info(struct sdl_progress_bar_s* self, struct sdl_scroll_s* scroll)
{
    set_render_color(g_sdl_text_color);
    debugf(g_sdl_renderer, scroll->x_p, newline(scroll), "%s %.1f", self->title, self->value);
    self->rect.x = scroll->x_p;
    self->rect.y = scroll->y_p;
    newline(scroll);
    draw_progress_bar(self);
    scroll_by(scroll, self->rect.h);
}

static void
draw_time_panel_info(struct sdl_time_panel_s* self, struct sdl_scroll_s* scroll)
{
    set_render_color(g_sdl_text_color);
    debugf(g_sdl_renderer, scroll->x_p, newline(scroll), "%s", self->title);
    for(size_t i = 0; i < g_sdl_time_panel_size; i++)
    {
        set_render_color(get_channel_color(i));
        float average = calc_slide_buffer_average(self->slide_buffer[i]);
        const char* label = self->labels[i];
        if(label)
        {
            debugf(g_sdl_renderer, scroll->x_p, newline(scroll), "%12s: %6.3f", label, average);
        }
    }
    self->rect.x = scroll->x_p;
    self->rect.y = scroll->y_p;
    draw_time_panel(self);
    newline(scroll);
    scroll_by(scroll, self->rect.h);
}

static void
draw_engine_info(const struct engine_s* engine, struct sdl_scroll_s* scroll)
{
    set_render_color(g_sdl_text_color);
    debugf(g_sdl_renderer, scroll->x_p, newline(scroll), "trigger_min_r_per_s: %.0f", g_sampler_min_angular_velocity_r_per_s);
    debugf(g_sdl_renderer, scroll->x_p, newline(scroll), "chamber_s_bytes: %ld", sizeof(struct chamber_s));
}

static void
draw_info_title(struct sdl_scroll_s* scroll)
{
    set_render_color(g_sdl_text_color);
    debugf(g_sdl_renderer, scroll->x_p, newline(scroll), "%s", g_sdl_title);
}

static void
draw_info(
    const struct engine_s* engine,
    struct sdl_time_panel_s* loop_time_panel,
    struct sdl_time_panel_s* engine_time_panel,
    struct sdl_time_panel_s* audio_buffer_time_panel,
    struct sdl_progress_bar_s* rad_per_sec_progress_bar,
    struct sdl_progress_bar_s* frames_per_sec_progress_bar)
{
    struct sdl_scroll_s scroll = {
        .x_p = compute_plot_column_width_p(engine) + g_sdl_line_spacing_p,
        .y_p = g_sdl_line_spacing_p,
    };
    draw_info_title(&scroll);
    draw_time_panel_info(loop_time_panel, &scroll);
    draw_time_panel_info(engine_time_panel, &scroll);
    draw_time_panel_info(audio_buffer_time_panel, &scroll);
    draw_progress_bar_info(rad_per_sec_progress_bar, &scroll);
    draw_progress_bar_info(frames_per_sec_progress_bar, &scroll);
    draw_engine_info(engine, &scroll);
}

static void
draw_demo_engine(const struct engine_s* engine)
{
    SDL_FPoint points[engine->size];
    compute_radials(engine, points);
    clear_screen();
    present(g_sdl_demo_delay_ms);
    for(size_t i = 0; i < engine->size; i++)
    {
        for(size_t next, j = 0; (next = engine->node[i].next[j]); j++)
        {
            draw_line_by(points[i], points[next]);
            struct node_s* x = &engine->node[i];
            struct node_s* y = &engine->node[j];
            draw_node_at(x, points[i], g_sdl_container_color);
            draw_node_at(y, points[j], g_sdl_container_color);
            present(g_sdl_demo_delay_ms);
        }
    }
}

static void
toggle_node_at(struct engine_s* engine, struct sampler_s* sampler, float x_p, float y_p)
{
    SDL_FPoint points[engine->size];
    compute_radials(engine, points);
    for(size_t i = 0; i < engine->size; i++)
    {
        struct node_s* node = &engine->node[i];
        SDL_FPoint point = points[i];
        SDL_FRect rect = { point.x, point.y, g_sdl_node_w_p, g_sdl_node_w_p };
        SDL_FPoint select = { x_p, y_p };
        if(SDL_PointInRectFloat(&select, &rect))
        {
            clear_sampler(sampler);
            node->is_selected ^= true;
            break;
        }
    }
}

static bool
handle_input(struct engine_s* engine, struct sampler_s* sampler)
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
            break;
        case SDLK_2:
            break;
        case SDLK_3:
            break;
        case SDLK_4:
            break;
        case SDLK_5:
            break;
        case SDLK_6:
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
        case SDLK_N:
            select_next(engine->node, engine->size);
            break;
        }
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        switch(event.button.button)
        {
        case SDL_BUTTON_LEFT:
            toggle_node_at(engine, sampler, event.button.x, event.button.y);
            break;
        case SDL_BUTTON_RIGHT:
            break;
        }
        break;
    }
    return false;
}
