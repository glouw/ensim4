constexpr double g_sdl_char_size_p = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;
constexpr double g_sdl_half_char_size_p = g_sdl_char_size_p / 2.0;
constexpr double g_sdl_line_spacing_p = 1.5 * g_sdl_char_size_p;

struct sdl_scroll_s
{
    double x_p;
    double y_p;
};

static double
scroll_by(struct sdl_scroll_s* self, double dy_p)
{
    double y_p = self->y_p;
    self->y_p += dy_p;
    return y_p;
}

static double
newline(struct sdl_scroll_s* self)
{
    return scroll_by(self, g_sdl_line_spacing_p);
}

static double
calc_scroll_newline_pixels_p(size_t newlines)
{
    return newlines * g_sdl_char_size_p + newlines * g_sdl_line_spacing_p;
}
