static constexpr size_t g_max_waves = 8;

struct wave_s
{
    double static_pressure_stage_pa[g_synth_buffer_size];
    double static_pressure_in_pa[g_synth_buffer_size];
    double static_pressure_out_pa[g_synth_buffer_size];
    size_t stage_index;
}
g_waves[g_max_waves];

static double g_wave_static_pressure_pa[g_synth_buffer_size];

static void
clear_waves()
{
    for(size_t i = 0; i < g_max_waves; i++)
    {
        for(size_t j = 0; j < g_synth_buffer_size; j++)
        {
            g_waves[i].static_pressure_stage_pa[j] = g_gas_ambient_static_pressure_pa;
        }
    }
}

static void
stage_wave(size_t wave_index, double static_pressure_pa)
{
    struct wave_s* self = &g_waves[wave_index];
    self->static_pressure_stage_pa[self->stage_index++] = static_pressure_pa;
}

static void
unstage_wave(size_t wave_index)
{
    struct wave_s* self = &g_waves[wave_index];
    for(size_t i = 0; i < g_synth_buffer_size; i++)
    {
        self->static_pressure_in_pa[i] = self->static_pressure_stage_pa[i];
    }
    self->stage_index = 0;
}

static void
run_wave(size_t wave_index)
{
    struct wave_s* self = &g_waves[wave_index];
    for(size_t i = 0; i < g_synth_buffer_size; i++)
    {
        self->static_pressure_out_pa[i] = self->static_pressure_in_pa[i];
    }
}
