static constexpr float sdl_char_size_p = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;
static constexpr float sdl_half_char_size_p = sdl_char_size_p / 2.0f;
static constexpr float sdl_line_spacing_p = 1.5f * sdl_char_size_p;

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
