constexpr char g_sdl_title[] = "ensim4";
constexpr bool g_sdl_use_full_screen = true;
constexpr double g_sdl_xres_p = 1920.0;
constexpr double g_sdl_yres_p = 1080.0;
constexpr double g_sdl_mid_x_p = g_sdl_xres_p / 2.0;
constexpr double g_sdl_mid_y_p = g_sdl_yres_p / 2.0;
constexpr double g_sdl_node_w_p = 32.0;
constexpr double g_sdl_node_half_w_p = g_sdl_node_w_p / 2.0;
constexpr double g_sdl_plot_lowpass_filter_hz = 1000.0;
constexpr double g_sdl_piston_scale_p_per_m = 400.0;
constexpr double g_sdl_piston_space_p = 4.0;
constexpr double g_sdl_zero_line_mix = 0.66;
constexpr size_t g_sdl_flow_cycle_spinner_divisor = 2048;
constexpr size_t g_sdl_max_display_samples = g_sampler_max_samples / 16;
constexpr size_t g_sdl_supported_widget_w_p = 192;

constexpr SDL_FColor g_sdl_channel_color[] = {
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

constexpr SDL_FColor g_sdl_panic_color = {
    1.00f, 0.00f, 0.00f, 1.0f
};

constexpr SDL_FColor g_sdl_black_color = {
    0.00f, 0.00f, 0.00f, 1.0f
};

constexpr SDL_FColor g_sdl_dark_line_color = {
    0.15f, 0.15f, 0.15f, 1.0f
};

constexpr SDL_FColor g_sdl_line_color = {
    0.28f, 0.28f, 0.28f, 1.0f
};

constexpr SDL_FColor g_sdl_container_color = {
    0.50f, 0.50f, 0.50f, 1.0f
};

constexpr SDL_FColor g_sdl_text_color = {
    1.00f, 1.00f, 1.00f, 1.0f
};

constexpr char g_sdl_spinner[][2] = { "|", "/", "-", "\\", "|", "/", "-", "\\" };

static SDL_Window* g_sdl_window = nullptr;
static SDL_Renderer* g_sdl_renderer = nullptr;

static void
set_render_color(SDL_FColor color)
{
    SDL_SetRenderDrawColorFloat(g_sdl_renderer, color.r, color.g, color.b, color.a);
}

static void
draw_rect(SDL_FRect rect, SDL_FColor color)
{
    set_render_color(color);
    SDL_RenderRect(g_sdl_renderer, &rect);
}

static void
draw_filled_rect(SDL_FRect rect, SDL_FColor color)
{
    set_render_color(color);
    SDL_RenderFillRect(g_sdl_renderer, &rect);
}

static void
draw_filled_outline_rect(SDL_FRect rect, SDL_FColor inner, SDL_FColor outer)
{
    draw_filled_rect(rect, inner);
    draw_rect(rect, outer);
}

static SDL_FPoint
add_point(SDL_FPoint self, SDL_FPoint other)
{
    self.x += other.x;
    self.y += other.y;
    return self;
}

static void
draw_line(SDL_FPoint from, SDL_FPoint to, SDL_FColor color)
{
    set_render_color(color);
    SDL_RenderLine(g_sdl_renderer, from.x, from.y, to.x, to.y);
}

static void
draw_lines(SDL_FPoint points[], size_t size, SDL_FColor color)
{
    set_render_color(color);
    SDL_RenderLines(g_sdl_renderer, points, size);
}

static void
draw_points(SDL_FPoint points[], size_t size, SDL_FColor color)
{
    set_render_color(color);
    SDL_RenderPoints(g_sdl_renderer, points, size);
}

static SDL_FPoint
calc_point_in_rect(double value, SDL_FRect rect, size_t index, size_t size)
{
    double border_p = 1.0;
    double x_p = rect.x + border_p;
    double y_p = rect.y + border_p;
    double w_p = rect.w - border_p * 2.0;
    double h_p = rect.h - border_p * 2.0;
    SDL_FPoint point = {
        x_p + (w_p * index) / (size - 1),
        y_p + h_p * (1.0 - value),
    };
    return point;
}

static void
draw_slide_buffer(sdl_slide_buffer_t self, SDL_FRect rect, double min_val, double max_val, SDL_FColor color)
{
    static SDL_FPoint points[g_sdl_slide_buffer_size];
    double range = max_val - min_val;
    for (size_t i = 0; i < g_sdl_slide_buffer_size; i++)
    {
        double normalized_value = (self[i] - min_val) / range;
        points[i] = calc_point_in_rect(normalized_value, rect, i, g_sdl_slide_buffer_size);
    }
    draw_lines(points, g_sdl_slide_buffer_size, color);
}

static void
draw_time_panel(struct sdl_time_panel_s* self)
{
    for(size_t i = 0; i < g_sdl_time_panel_size; i++)
    {
        const char* label = self->labels[i];
        if(label)
        {
            draw_slide_buffer(self->slide_buffer[i], self->rect, self->min_value, self->max_value, get_channel_color(i));
        }
    }
    draw_rect(self->rect, g_sdl_container_color);
}

static void
draw_progress_bar(struct sdl_progress_bar_s* self)
{
    double percent = (self->max_value > 0.0) ? (self->value / self->max_value) : 0.0;
    percent = clamp(percent, 0.0, 1.0);
    SDL_FRect fill = self->rect;
    fill.w = self->rect.w * percent;
    draw_filled_rect(fill, get_channel_color(2));
    draw_rect(self->rect, g_sdl_container_color);
}

static void
init_sdl()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_CreateWindowAndRenderer(
        g_sdl_title,
        g_sdl_xres_p,
        g_sdl_yres_p,
        g_sdl_use_full_screen ? SDL_WINDOW_FULLSCREEN : 0,
        &g_sdl_window,
        &g_sdl_renderer);
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
present_renderer()
{
    SDL_RenderPresent(g_sdl_renderer);
}

static double
calc_radial_radius_p(struct engine_s* engine)
{
    return engine->radial_spacing * (engine->size * g_sdl_node_w_p) / (2.0 * g_std_pi_r);
}

static double
calc_radial_diameter_p(struct engine_s* engine)
{
    return 2.0 * calc_radial_radius_p(engine);
}

static double
calc_plot_column_width_p(struct engine_s* engine)
{
    return (g_sdl_xres_p - calc_radial_diameter_p(engine)) / 2.0 - 1.4 * g_sdl_supported_widget_w_p;
}

static SDL_FPoint
center_text(SDL_FPoint point, const char* text)
{
    size_t len = strlen(text);
    double x_p = point.x + g_sdl_node_half_w_p - g_sdl_half_char_size_p * len;
    double y_p = point.y + g_sdl_node_half_w_p - g_sdl_half_char_size_p;
    return (SDL_FPoint) { x_p, y_p };
}

static void
draw_node_at(struct node_s* node, SDL_FPoint point, SDL_FColor color)
{
    SDL_FRect rect = { point.x, point.y, g_sdl_node_w_p, g_sdl_node_w_p };
    draw_filled_outline_rect(rect, node->as.chamber.should_panic ? color : g_sdl_black_color, color);
    size_t cycle = node->as.chamber.flow_cycles / g_sdl_flow_cycle_spinner_divisor;
    size_t index = cycle % len(g_sdl_spinner);
    const char* spinner = g_sdl_spinner[index];
    SDL_FPoint mid = point;
    mid = center_text(mid, spinner);
    set_render_color(g_sdl_text_color);
    SDL_RenderDebugTextFormat(g_sdl_renderer, mid.x, mid.y, "%s", spinner);
}

static void
calc_radials(struct engine_s* engine, SDL_FPoint points[], size_t size)
{
    double radius_p = calc_radial_radius_p(engine);
    for(size_t i = 0; i < size; i++)
    {
        double theta_r = 2.0 * g_std_pi_r * i / size;
        points[i].x = g_sdl_mid_x_p + radius_p * cos(theta_r) - g_sdl_node_half_w_p;
        points[i].y = g_sdl_mid_y_p + radius_p * sin(theta_r) - g_sdl_node_half_w_p;
    }
}

static SDL_FColor
mix_fcolors(SDL_FColor self, SDL_FColor other, double ratio)
{
    SDL_FColor result;
    result.r = self.r + (other.r - self.r) * ratio;
    result.g = self.g + (other.g - self.g) * ratio;
    result.b = self.b + (other.b - self.b) * ratio;
    result.a = self.a + (other.a - self.a) * ratio;
    return result;
}

static void
draw_radial_lines(struct engine_s* engine, SDL_FPoint points[], size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        struct node_s* node = &engine->node[i];
        for(size_t next, j = 0; (next = node->next[j]); j++)
        {
            struct SDL_FPoint shift = { g_sdl_node_half_w_p, g_sdl_node_half_w_p };
            struct SDL_FPoint from = add_point(points[i], shift);
            struct SDL_FPoint to = add_point(points[next], shift);
            SDL_FColor open_color = mix_fcolors(
                g_sdl_dark_line_color,
                g_sdl_line_color,
                node->as.chamber.nozzle_open_ratio);
            SDL_FColor final_color = node->is_next_selected
                ? get_channel_color(6)
                : open_color;
            draw_line(from, to, final_color);
        }
    }
}

static void
draw_radial_nodes(struct engine_s* engine, SDL_FPoint points[], size_t size)
{
    size_t color_index = 0;
    for(size_t i = 0; i < size; i++)
    {
        struct node_s* node = &engine->node[i];
        SDL_FPoint point = points[i];
        if(node->is_selected)
        {
            draw_node_at(node, point, get_channel_color(color_index));
            color_index += count_node_edges(node);
        }
        else
        {
            draw_node_at(node, point, g_sdl_container_color);
        }
    }
}

static void
draw_radial_names(struct engine_s* engine, SDL_FPoint points[], size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        struct node_s* node = &engine->node[i];
        const char* node_name = g_node_name_string[node->type];
        SDL_FPoint point = points[i];
        point = center_text(point, node_name);
        point.y -= g_sdl_line_spacing_p;
        point.y -= g_sdl_node_half_w_p;
        set_render_color(g_sdl_text_color);
        SDL_RenderDebugTextFormat(g_sdl_renderer, point.x, point.y, "%s", node_name);
    }
}

static void
draw_radial_chambers(struct engine_s* engine)
{
    size_t size = engine->size;
    SDL_FPoint points[size];
    calc_radials(engine, points, size);
    draw_radial_lines(engine, points, size);
    draw_radial_nodes(engine, points, size);
    draw_radial_names(engine, points, size);
}

static void
cleanup_samples(double samples[], size_t size)
{
    struct lowpass_filter_s filter = { .last = samples[0] };
    for(size_t i = 0; i < size; i++)
    {
        samples[i] = filter_lowpass(&filter, g_sdl_plot_lowpass_filter_hz, samples[i]);
    }
}

static void
copy_samples(double samples[], double other[], size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        samples[i] = other[i];
    }
}

static size_t
down_sample_samples(double samples[], size_t size, size_t cap, size_t* step)
{
    if(size > cap)
    {
        *step = (size + cap - 1) / cap;
        size_t j = 0;
        for(size_t i = 0; i < size; i += *step)
        {
            samples[j++] = samples[i];
        }
        return j;
    }
    return size;
}

static void
draw_zero_line(struct SDL_FRect rect, struct normalized_s* normalized, SDL_FColor color)
{
    double zero_line_offset_p = rect.h * calc_normalized_zero_offset_ratio(normalized);
    double zero_line_y_p = rect.y + zero_line_offset_p;
    bool in_bounds = zero_line_y_p > rect.y && zero_line_y_p < rect.y + rect.h;
    if(in_bounds)
    {
        SDL_FPoint a = { rect.x, zero_line_y_p };
        SDL_FPoint b = { rect.x + rect.w, zero_line_y_p };
        SDL_FColor zero_line_color = mix_fcolors(color, g_sdl_black_color, g_sdl_zero_line_mix);
        draw_line(a, b, zero_line_color);
    }
}

static void
draw_plot_channel(SDL_FRect rects[], size_t channel, struct sampler_s* sampler, bool use_plot_filter)
{
    size_t buffered = 0;
    constexpr size_t max_buffer_size = g_sample_name_e_size * g_sdl_max_display_samples;
    static SDL_FPoint buffer[max_buffer_size];
    static double samples[g_sampler_max_samples];
    SDL_FColor color = get_channel_color(channel);
    for(enum sample_name_e sample_name = 0; sample_name < g_sample_name_e_size; sample_name++)
    {
        size_t step = 0;
        size_t sampler_size = sampler->size;
        copy_samples(samples, sampler->channel[channel][sample_name], sampler_size);
        sampler_size = down_sample_samples(samples, sampler_size, g_sdl_max_display_samples, &step);
        struct normalized_s normalized = normalize_samples(samples, sampler_size);
        if(use_plot_filter)
        {
            cleanup_samples(samples, sampler_size);
        }
        SDL_FRect rect = rects[sample_name];
        struct
        {
            const char* name;
            double value;
        }
        lines[] = {
            { "max: %+.3e", normalized.max_value },
            { "min: %+.3e", normalized.min_value },
            { "div: %3.3f", normalized.div_value },
#if 0
            { "stp: %0.0f", step },
            { "avg: %+.3e", normalized.avg_value },
#endif
        };
        bool is_last_channel = channel == sampler->channel_index - 1;
        if(is_last_channel)
        {
            struct sdl_scroll_s scroll = {
                rect.x + 1 * g_sdl_line_spacing_p,
                rect.y + 2 * g_sdl_line_spacing_p,
            };
            for(size_t i = 0; i < len(lines); i++)
            {
                set_render_color(color);
                SDL_RenderDebugTextFormat(g_sdl_renderer, scroll.x_p, newline(&scroll), lines[i].name, lines[i].value);
            }
        }
        if(normalized.is_success)
        {
            size_t data_rect_y_offset_p = calc_scroll_newline_pixels_p(len(lines));
            SDL_FRect data_rect = {
                rect.x, rect.y + data_rect_y_offset_p,
                rect.w, rect.h - data_rect_y_offset_p,
            };
            draw_zero_line(data_rect, &normalized, color);
            for(size_t i = 0; i < sampler_size; i++)
            {
                buffer[buffered++] = calc_point_in_rect(samples[i], data_rect, i, sampler_size);
            }
        }
    }
    draw_points(buffer, buffered, color);
}

static void
draw_plot_channels(SDL_FRect rects[], struct sampler_s* sampler, size_t channels, bool use_plot_filter)
{
    for(size_t channel = 0; channel < channels; channel++)
    {
        draw_plot_channel(rects, channel, sampler, use_plot_filter);
    }
}

static void
draw_plot_container(SDL_FRect rect, const char* name_string)
{
    draw_rect(rect, g_sdl_container_color);
    double x_p = rect.x + g_sdl_line_spacing_p;
    double y_p = rect.y + g_sdl_line_spacing_p;
    set_render_color(g_sdl_text_color);
    SDL_RenderDebugTextFormat(g_sdl_renderer, x_p, y_p, "%s", name_string);
}

static void
draw_plot_containers(SDL_FRect rects[])
{
    for(enum sample_name_e name = 0; name < g_sample_name_e_size; name++)
    {
        SDL_FRect rect = rects[name];
        draw_plot_container(rect, skip_sample_namespace(g_sample_name_string[name]));
    }
}

static void
position_plot_container(double x_p, double w_p, double h_p, enum sample_name_e* name, SDL_FRect rects[])
{
    double y_p = 0.0;
    while(y_p < g_sdl_yres_p)
    {
        rects[(*name)++] = (SDL_FRect) { x_p, y_p, w_p, h_p };
        y_p += h_p;
    }
}

static void
position_plot_containers(struct engine_s* engine, SDL_FRect rects[])
{
    enum sample_name_e sample_name = 0;
    double w_p = calc_plot_column_width_p(engine);
    double samples_per_column = g_sample_name_e_size / 2.0;
    double left_samples = floor(samples_per_column);
    double right_samples = ceil(samples_per_column);
    double left_h = g_sdl_yres_p / left_samples;
    double right_h = g_sdl_yres_p / right_samples;
    position_plot_container(0.0, w_p, left_h, &sample_name, rects);
    position_plot_container(g_sdl_xres_p - w_p, w_p, right_h, &sample_name, rects);
}

static void
draw_plots(struct engine_s* engine, struct sampler_s* sampler)
{
    static SDL_FRect rects[g_sample_name_e_size];
    position_plot_containers(engine, rects);
    draw_plot_channels(rects, sampler, sampler->channel_index, engine->use_plot_filter);
    draw_plot_containers(rects);
}

static void
draw_progress_bar_info(struct sdl_progress_bar_s* self, struct sdl_scroll_s* scroll)
{
    set_render_color(g_sdl_text_color);
    SDL_RenderDebugTextFormat(g_sdl_renderer, scroll->x_p, newline(scroll), "%s %.2f", self->title, self->value);
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
    SDL_RenderDebugTextFormat(g_sdl_renderer, scroll->x_p, newline(scroll), "%s", self->title);
    for(size_t i = 0; i < g_sdl_time_panel_size; i++)
    {
        double average = calc_slide_buffer_average(self->slide_buffer[i]);
        const char* label = self->labels[i];
        if(label)
        {
            set_render_color(get_channel_color(i));
            SDL_RenderDebugTextFormat(g_sdl_renderer, scroll->x_p, newline(scroll), "%12s: %6.3f", label, average);
        }
    }
    self->rect.x = scroll->x_p;
    self->rect.y = scroll->y_p;
    draw_time_panel(self);
    newline(scroll);
    scroll_by(scroll, self->rect.h);
}

static void
draw_general_info(struct engine_s* engine, struct sdl_scroll_s* scroll)
{
    SDL_FColor warning = get_channel_color(0);
    SDL_FColor simple = g_sdl_text_color;
    struct
    {
        const char* name;
        double value;
        SDL_FColor color;
    }
    lines[] = {
        { "trigger_min_r_per_s: %.0f" , g_sampler_min_angular_velocity_r_per_s , engine->crankshaft.angular_velocity_r_per_s < g_sampler_min_angular_velocity_r_per_s ? warning : simple },
        { "monitor_hz: %.0f"          , g_std_monitor_refresh_rate             , simple },
        { "g_engine_node_bytes: %.0f" , sizeof(g_engine_node)                  , simple },
        { "g_engine_nodes: %.0f"      , len(g_engine_node)                     , simple },
        { "supported_channels: %.0f"  , g_sampler_max_channels                 , simple },
    };
    for(size_t i = 0; i < len(lines); i++)
    {
        set_render_color(lines[i].color);
        SDL_RenderDebugTextFormat(g_sdl_renderer, scroll->x_p, newline(scroll), lines[i].name, lines[i].value);
    }
}

static void
draw_info_title(struct engine_s* engine, struct sdl_scroll_s* scroll)
{
    SDL_FColor active = get_channel_color(0);
    SDL_FColor simple = g_sdl_text_color;
    struct
    {
        const char* name;
        SDL_FColor color;
    }
    lines[] = {
        { g_sdl_title                   , simple                                    },
        { "the inline engine simulator" , simple                                    },
        { "    t: use_convolution"      , engine->use_convolution ? active : simple },
        { "    y: use_cfd"              , engine->use_cfd         ? active : simple },
        { "    u: use_plot_filter"      , engine->use_plot_filter ? active : simple },
        { "    d: ignition_on"          , engine->can_ignite      ? active : simple },
        { "space: starter_on"           , engine->starter.is_on   ? active : simple },
        { "------ nodes --------------" , simple                                    },
        { "    c: clear"                , simple                                    },
        { "    n: next (from one)"      , simple                                    },
        { "    i: intakes"              , simple                                    },
        { "    e: exhausts"             , simple                                    },
        { "    p: pistons"              , simple                                    },
        { ""                            , simple                                    },
    };
    for(size_t i = 0; i < len(lines); i++)
    {
        set_render_color(lines[i].color);
        SDL_RenderDebugTextFormat(g_sdl_renderer, scroll->x_p, newline(scroll), "%s", lines[i].name);
    }
}

static void
draw_left_info(
    struct engine_s* engine,
    struct sdl_time_panel_s* loop_time_panel,
    struct sdl_time_panel_s* engine_time_panel,
    struct sdl_time_panel_s* audio_buffer_time_panel,
    struct sdl_progress_bar_s* frames_per_sec_progress_bar)
{
    struct sdl_scroll_s scroll = {
        .x_p = calc_plot_column_width_p(engine) + g_sdl_line_spacing_p,
        .y_p = g_sdl_line_spacing_p,
    };
    draw_info_title(engine, &scroll);
    draw_time_panel_info(loop_time_panel, &scroll);
    draw_time_panel_info(engine_time_panel, &scroll);
    draw_time_panel_info(audio_buffer_time_panel, &scroll);
    draw_progress_bar_info(frames_per_sec_progress_bar, &scroll);
    draw_general_info(engine, &scroll);
}

static void
draw_panel(struct sdl_panel_s* self, SDL_FRect rect, SDL_FColor color)
{
    static double samples[g_sampler_max_samples];
    static SDL_FPoint points[g_sdl_max_display_samples];
    draw_rect(self->rect, self->panic ? g_sdl_panic_color : g_sdl_container_color);
    size_t size = self->size;
    if(size > 0)
    {
        size_t step = 0;
        copy_samples(samples, self->sample, size);
        size = down_sample_samples(samples, size, g_sdl_max_display_samples, &step);
        for(size_t i = 0; i < size; i++)
        {
            points[i] = calc_point_in_rect(samples[i], rect, i, size);
        }
        draw_points(points, size, color);
    }
}

static void
draw_panel_info(struct sdl_panel_s* self, struct sdl_scroll_s* scroll)
{
    double x_p = scroll->x_p;
    set_render_color(g_sdl_text_color);
    SDL_RenderDebugTextFormat(g_sdl_renderer, x_p, newline(scroll), "%s", self->title);
    struct
    {
        const char* name;
        double value;
    }
    lines[] = {
        { "max %f", self->normalized.is_success ? self->normalized.max_value : 0.0 },
        { "min %f", self->normalized.is_success ? self->normalized.min_value : 0.0 },
    };
    SDL_FColor color = get_channel_color(6);
    for(size_t i = 0; i < len(lines); i++)
    {
        set_render_color(color);
        SDL_RenderDebugTextFormat(g_sdl_renderer, x_p, newline(scroll), lines[i].name, lines[i].value);
    }
    self->rect.x = x_p;
    self->rect.y = scroll->y_p;
    draw_panel(self, self->rect, color);
    newline(scroll);
    scroll_by(scroll, self->rect.h);
}

static void
draw_right_info_waves(
    struct engine_s* engine,
    struct sdl_panel_s wave_panel[],
    size_t wave_panel_size,
    struct sdl_scroll_s* scroll)
{
    size_t wave_index = 0;
    for(size_t i = 0; i < engine->size; i++)
    {
        struct node_s* node = &engine->node[i];
        if(node->type == g_is_eplenum)
        {
            struct eplenum_s* eplenum = &node->as.eplenum;
            struct wave_s* wave = &g_wave_table[eplenum->wave_index];
            if(wave_index == wave_panel_size)
            {
                break;
            }
            struct sdl_panel_s* panel = &wave_panel[wave_index];
            push_panel_prim(panel, wave->solver.prim, g_wave_cells);
            draw_panel_info(panel, scroll);
            struct
            {
                const char* name;
                double value;
            }
            lines[] = {
                { "max_m_per_s %.1f", wave->solver.max_wave_speed_m_per_s },
                { "pipe_len_m %.2f", wave->solver.pipe_length_m },
                { "mic_position_ratio %.2f", wave->solver.mic_position_ratio },
            };
            for(size_t i = 0; i < len(lines); i++)
            {
                SDL_RenderDebugTextFormat(g_sdl_renderer, scroll->x_p, newline(scroll), lines[i].name, lines[i].value);
            }
            newline(scroll);
            wave_index++;
        }
    }
}

static void
draw_right_info(
    struct engine_s* engine,
    struct sdl_panel_s* starter_panel_r_per_s,
    struct sdl_panel_s* convolution_panel_time_domain,
    struct sdl_progress_bar_s* rad_per_sec_progress_bar,
    struct sdl_panel_s wave_panel[],
    size_t wave_panel_size,
    struct sdl_panel_s* synth_sample_panel,
    struct sdl_progress_bar_s* throttle_progress_bar)
{
    struct sdl_scroll_s scroll = {
        .x_p = g_sdl_xres_p - calc_plot_column_width_p(engine) - g_sdl_line_spacing_p - g_sdl_supported_widget_w_p,
        .y_p = g_sdl_line_spacing_p,
    };
    draw_panel_info(starter_panel_r_per_s, &scroll);
    draw_panel_info(convolution_panel_time_domain, &scroll);
    draw_progress_bar_info(rad_per_sec_progress_bar, &scroll);
    draw_right_info_waves(engine, wave_panel, wave_panel_size, &scroll);
    draw_panel_info(synth_sample_panel, &scroll);
    draw_progress_bar_info(throttle_progress_bar, &scroll);
}

static void
toggle_node_at(struct engine_s* engine, struct sampler_s* sampler, double x_p, double y_p)
{
    size_t size = engine->size;
    remove_next_selected(engine->node, size);
    SDL_FPoint points[size];
    calc_radials(engine, points, size);
    for(size_t i = 0; i < size; i++)
    {
        struct node_s* node = &engine->node[i];
        node->is_next_selected = false;
        SDL_FPoint point = points[i];
        SDL_FRect rect = { point.x, point.y, g_sdl_node_w_p, g_sdl_node_w_p };
        SDL_FPoint select = { x_p, y_p };
        if(SDL_PointInRectFloat(&select, &rect))
        {
            clear_channel_sampler(sampler);
            node->is_selected ^= true;
            break;
        }
    }
}

static void
draw_pistons(struct engine_s* engine)
{
    double x_p = g_sdl_mid_x_p;
    double y_p = g_sdl_mid_y_p - 32.0;
    set_render_color(g_sdl_text_color);
    SDL_RenderDebugTextFormat(g_sdl_renderer, x_p, y_p, "%s", engine->name);
    y_p += calc_scroll_newline_pixels_p(1);
    for(size_t i = 0; i < engine->size; i++)
    {
        struct node_s* node = &engine->node[i];
        if(node->type == g_is_piston)
        {
            struct piston_s* piston = &node->as.piston;
            SDL_FRect head = {
                x_p,
                g_sdl_piston_scale_p_per_m * calc_piston_chamber_depth_m(piston) + y_p,
                g_sdl_piston_scale_p_per_m * piston->diameter_m,
                g_sdl_piston_scale_p_per_m * piston->head_compression_height_m * 2.0,
            };
            SDL_FPoint block_line_a = {
                x_p,
                y_p,
            };
            SDL_FPoint block_line_b = {
                x_p + head.w,
                y_p,
            };
            draw_line(block_line_a, block_line_b, g_sdl_container_color);
            double conrod_w_p = head.w / 4.0;
            SDL_FRect conrod = {
                head.x + (head.w - conrod_w_p) / 2.0,
                head.y + head.h,
                conrod_w_p,
                g_sdl_piston_scale_p_per_m * piston->connecting_rod_length_m,
            };
            if(is_sparkplug_enabled(&piston->sparkplug, &engine->crankshaft))
            {
                draw_filled_outline_rect(head, get_channel_color(3), g_sdl_container_color);
            }
            draw_rect(head, g_sdl_container_color);
            draw_rect(conrod, g_sdl_container_color);
            x_p += g_sdl_piston_space_p + head.w;
        }
    }
}

static void
draw_panic_message()
{
    if(g_panic_message != nullptr)
    {
        SDL_FPoint point = {
            .x = g_sdl_mid_x_p
        };
        point = center_text(point, g_panic_message);
        point.y += g_sdl_line_spacing_p;
        set_render_color(g_sdl_panic_color);
        SDL_RenderDebugTextFormat(g_sdl_renderer, point.x, point.y, "%s", g_panic_message);
    }
}

static void
draw_to_renderer(
    struct engine_s* engine,
    struct sampler_s* sampler,
    struct sdl_time_panel_s* loop_time_panel,
    struct sdl_time_panel_s* engine_time_panel,
    struct sdl_time_panel_s* audio_buffer_time_panel,
    struct sdl_progress_bar_s* r_per_s_progress_bar,
    struct sdl_progress_bar_s* frames_per_sec_progress_bar,
    struct sdl_progress_bar_s* throttle_progress_bar,
    struct sdl_panel_s* starter_panel_r_per_s,
    struct sdl_panel_s* convolution_panel_time_domain,
    struct sdl_panel_s wave_panel[],
    size_t wave_panel_size,
    struct sdl_panel_s* synth_sample_panel)
{
    clear_screen();
    draw_plots(engine, sampler);
    draw_radial_chambers(engine);
    draw_left_info(engine, loop_time_panel, engine_time_panel, audio_buffer_time_panel, frames_per_sec_progress_bar);
    draw_right_info(engine, starter_panel_r_per_s, convolution_panel_time_domain, r_per_s_progress_bar, wave_panel, wave_panel_size, synth_sample_panel, throttle_progress_bar);
    draw_pistons(engine);
    draw_panic_message();
}

static bool
handle_input(struct engine_s* engine, struct sampler_s* sampler)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_EVENT_QUIT:
            return true;
        case SDL_EVENT_KEY_DOWN:
            switch(event.key.key)
            {
            case SDLK_SPACE:
                engine->starter.is_on = true;
                break;
            case SDLK_D:
                engine->can_ignite ^= true;
                break;
            case SDLK_H:
                engine->throttle_open_ratio = engine->no_throttle;
                break;
            case SDLK_J:
                engine->throttle_open_ratio = engine->low_throttle;
                break;
            case SDLK_K:
                engine->throttle_open_ratio = engine->mid_throttle;
                break;
            case SDLK_L:
                engine->throttle_open_ratio = engine->high_throttle;
                break;
            case SDLK_Y:
                enable_engine_cfd(engine, engine->use_cfd ^= true);
                break;
            case SDLK_U:
                engine->use_plot_filter ^= true;
                break;
            case SDLK_T:
                engine->use_convolution ^= true;
                break;
            }
            break;
        case SDL_EVENT_KEY_UP:
            switch(event.key.key)
            {
            case SDLK_SPACE:
                engine->starter.is_on = false;
                break;
            case SDLK_P:
                deselect_all_nodes(engine->node, engine->size);
                select_nodes(engine->node, engine->size, g_is_piston);
                break;
            case SDLK_I:
                deselect_all_nodes(engine->node, engine->size);
                select_nodes(engine->node, engine->size, g_is_afilter);
                select_nodes(engine->node, engine->size, g_is_throttle);
                select_nodes(engine->node, engine->size, g_is_iplenum);
                select_nodes(engine->node, engine->size, g_is_irunner);
                break;
            case SDLK_E:
                deselect_all_nodes(engine->node, engine->size);
                select_nodes(engine->node, engine->size, g_is_eplenum);
                select_nodes(engine->node, engine->size, g_is_erunner);
                select_nodes(engine->node, engine->size, g_is_exhaust);
                break;
            case SDLK_C:
                deselect_all_nodes(engine->node, engine->size);
                break;
            case SDLK_N:
                select_next(engine->node, engine->size);
                break;
            }
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
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
    }
    return false;
}
