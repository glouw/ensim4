constexpr int EXHAUST_CELL_COUNT = 512;

struct wave_s
{
    double pressure_current_pa[EXHAUST_CELL_COUNT];
};

static void
wave_update(struct wave_s* self, double static_gauge_pressure_pa)
{
    self->pressure_current_pa[EXHAUST_CELL_COUNT - 1] = static_gauge_pressure_pa;
}

static double
sample_wave_static_gauge_pressure_pa(struct wave_s* self)
{
    return self->pressure_current_pa[EXHAUST_CELL_COUNT - 1];
}
