struct sdl_panel_s
{
    const char* title;
    SDL_FRect rect;
    double sample[g_sampler_max_samples];
    size_t size;
    struct normalized_s normalized;
    bool panic;
};

static void
clear_panel(struct sdl_panel_s* self)
{
    clear(self->sample);
    self->size = 0;
}

static void
push_panel(struct sdl_panel_s* self, double value[], size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        self->sample[i] = value[i];
    }
    self->size = size;
    self->normalized = normalize_samples(self->sample, self->size);
}

static void
push_panel_double(struct sdl_panel_s* self, const double value[], size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        self->sample[i] = value[i];
    }
    self->size = size;
    self->normalized = normalize_samples(self->sample, self->size);
}

static void
push_panel_prim(struct sdl_panel_s* self, struct wave_prim_s prim[], size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        self->sample[i] = prim[i].p;
    }
    self->size = size;
    self->normalized = normalize_samples(self->sample, self->size);
    self->panic = self->normalized.is_success == false;
}
