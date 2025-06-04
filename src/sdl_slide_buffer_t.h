static constexpr size_t sdl_slide_buffer_size = 128;

typedef float sdl_slide_buffer_t[sdl_slide_buffer_size];

static float
calc_slide_buffer_average(const sdl_slide_buffer_t self)
{
    float average = 0.0;
    for(size_t i = 0; i < sdl_slide_buffer_size; i++)
    {
        average += self[i];
    }
    return average / sdl_slide_buffer_size;
}

static void
push_slide_buffer(sdl_slide_buffer_t self, float value)
{
    for(size_t i = 0; i < sdl_slide_buffer_size - 1; i++)
    {
        self[i] = self[i + 1];
    }
    self[sdl_slide_buffer_size - 1] = value;
}

