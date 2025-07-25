struct sdl_panel_s
{
    const char* title;
    SDL_FRect rect;
    float sample[g_sampler_max_samples];
    size_t size;
    struct normalized_s normalized;
};

static void
push_panel(struct sdl_panel_s* self, float value[], size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        self->sample[i] = value[i];
    }
    self->size = size;
    self->normalized = normalize_samples(self->sample, self->size);
}

static void
push_panel_double(struct sdl_panel_s* self, double value[], size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        self->sample[i] = value[i];
    }
    self->size = size;
    self->normalized = normalize_samples(self->sample, self->size);
}
