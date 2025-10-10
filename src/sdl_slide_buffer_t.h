constexpr size_t g_sdl_slide_buffer_size = 128;

typedef double sdl_slide_buffer_t[g_sdl_slide_buffer_size];

static double
calc_slide_buffer_average(sdl_slide_buffer_t self)
{
    double average = 0.0;
    for(size_t i = 0; i < g_sdl_slide_buffer_size; i++)
    {
        average += self[i];
    }
    return average / g_sdl_slide_buffer_size;
}

static void
push_slide_buffer(sdl_slide_buffer_t self, double value)
{
    for(size_t i = 0; i < g_sdl_slide_buffer_size - 1; i++)
    {
        self[i] = self[i + 1];
    }
    self[g_sdl_slide_buffer_size - 1] = value;
}
