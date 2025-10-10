constexpr size_t g_sdl_time_panel_size = 8;

struct sdl_time_panel_s
{
    const char* title;
    const char* labels[g_sdl_time_panel_size];
    SDL_FRect rect;
    double min_value;
    double max_value;
    sdl_slide_buffer_t slide_buffer[g_sdl_time_panel_size];
};

static void
push_time_panel(struct sdl_time_panel_s* self, double sample[])
{
    for(size_t i = 0; i < g_sdl_time_panel_size; i++)
    {
        if(self->labels[i] != nullptr)
        {
            push_slide_buffer(self->slide_buffer[i], sample[i]);
        }
    }
}
