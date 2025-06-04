static constexpr size_t sdl_time_panel_size = 4;

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
push_time_panel(struct sdl_time_panel_s* self, float sample[])
{
    for(size_t i = 0; i < sdl_time_panel_size; i++)
    {
        push_slide_buffer(self->slide_buffer[i], sample[i]);
    }
}
