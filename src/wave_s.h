static constexpr size_t g_wave_max_waves = 8;
static constexpr size_t g_wave_cells = 128;
static constexpr size_t g_wave_substeps = 4;
static constexpr size_t g_wave_sample_rate_hz = g_std_audio_sample_rate_hz * g_wave_substeps;
static constexpr double g_wave_gamma = 1.4;
static constexpr double g_wave_dt_s = 1.0 / g_wave_sample_rate_hz;
static constexpr double g_wave_pipe_length_m = 1.0;
static constexpr double g_wave_dx_m = g_wave_pipe_length_m / g_wave_cells;
static constexpr double g_wave_max_wave_speed_m_per_s = g_wave_dx_m / g_wave_dt_s;
static constexpr double g_wave_mic_position_ratio = 0.5;

struct wave_prim_s
{
    double rho;
    double u;
    double p;
};

struct wave_cons_s
{
    double rho;
    double mom;
    double en;
};

struct wave_flux_s
{
    double rho;
    double mom;
    double en;
};

struct wave_hllc_s
{
    struct wave_prim_s prim[g_wave_cells];
    struct wave_cons_s cons[g_wave_cells];
    struct wave_flux_s flux[g_wave_cells + 1];
};

struct wave_data_s
{
    struct wave_prim_s buffer0[g_synth_buffer_size];
    struct wave_prim_s buffer1[g_synth_buffer_size];
    double wave_sub_buffer_pa[g_synth_buffer_size];
    size_t index;
};

struct wave_s
{
    struct wave_data_s data;
    struct wave_hllc_s hllc;
}
g_waves[g_wave_max_waves];

static double g_wave_buffer_pa[g_synth_buffer_size];

static constexpr struct wave_prim_s g_ambient_wave_cell = {
    .p = g_gas_ambient_static_pressure_pa,
    .rho = g_gas_ambient_static_density_kg_per_m3,
    .u = 0.0,
};

static struct wave_cons_s
prim_to_cons(struct wave_prim_s self)
{
    return (struct wave_cons_s) {
        .rho = self.rho,
        .mom = self.rho * self.u,
        .en = self.p / (g_wave_gamma - 1.0) + 0.5 * self.rho * self.u * self.u,
    };
}

static struct wave_prim_s
cons_to_prim(struct wave_cons_s self)
{
    double u = self.mom / self.rho;
    return (struct wave_prim_s) {
        .rho = self.rho,
        .u = self.mom / self.rho,
        .p = (self.en - 0.5 * self.mom * u) * (g_wave_gamma - 1.0),
    };
}

static inline double
calc_cell_speed_of_sound(struct wave_prim_s self)
{
    return sqrt(g_wave_gamma * self.p / self.rho);
}

static struct wave_flux_s
calc_flux(struct wave_prim_s self)
{
    double e = self.p / (g_wave_gamma - 1.0) + 0.5 * self.rho * self.u * self.u;
    return (struct wave_flux_s) {
        .rho = self.rho * self.u,
        .mom = self.rho * self.u * self.u + self.p,
        .en = (e + self.p) * self.u,
    };
}

static struct wave_flux_s
calc_hllc_flux_component(
    struct wave_cons_s u,
    struct wave_flux_s f_base,
    struct wave_prim_s q,
    double s,
    double s_star)
{
    double coeff = u.rho * (s - q.u) / (s - s_star);
    double e_star = coeff * ((u.en / u.rho) + (s_star - q.u) * (s_star + q.p / (u.rho * (s - q.u))));
    return (struct wave_flux_s) {
        .rho = f_base.rho + s * (coeff - u.rho),
        .mom = f_base.mom + s * (coeff * s_star - u.mom),
        .en = f_base.en + s * (e_star - u.en),
    };
}

static double
calc_s_star(
    struct wave_prim_s ql,
    struct wave_prim_s qr,
    struct wave_cons_s ul,
    struct wave_cons_s ur,
    double sl,
    double sr)
{
    double term1 = qr.p - ql.p + ul.mom * (sl - ql.u) - ur.mom * (sr - qr.u);
    double term2 = ul.rho * (sl - ql.u) - ur.rho * (sr - qr.u);
    return term1 / term2;
}

static struct wave_flux_s
calc_hllc_flux(struct wave_prim_s ql, struct wave_prim_s qr)
{
    double cl = calc_cell_speed_of_sound(ql);
    double cr = calc_cell_speed_of_sound(qr);
    double sl = fmin(ql.u - cl, qr.u - cr);
    double sr = fmax(ql.u + cl, qr.u + cr);
    struct wave_cons_s ul = prim_to_cons(ql);
    struct wave_cons_s ur = prim_to_cons(qr);
    struct wave_flux_s fl = calc_flux(ql);
    struct wave_flux_s fr = calc_flux(qr);
    if(sl >= 0.0)
    {
        return fl;
    }
    if(sr <= 0.0)
    {
        return fr;
    }
    double s_star = calc_s_star(ql, qr, ul, ur, sl, sr);
    if(s_star >= 0.0)
    {
        return calc_hllc_flux_component(ul, fl, ql, sl, s_star);
    }
    else
    {
        return calc_hllc_flux_component(ur, fr, qr, sr, s_star);
    }
}

static void
compute_wave_flux(struct wave_hllc_s* self)
{
    size_t l = 0;
    size_t r = g_wave_cells;
    size_t z = r - 1;
    self->flux[l] = calc_hllc_flux(self->prim[l], self->prim[l]);
    self->flux[r] = calc_hllc_flux(self->prim[z], self->prim[z]);
    for(size_t i = 1; i < g_wave_cells; i++)
    {
        size_t x = i - 1;
        size_t y = i;
        self->flux[y] = calc_hllc_flux(self->prim[x], self->prim[y]);
    }
}

static void
update_wave_state(struct wave_hllc_s* self)
{
    for(size_t i = 1; i < g_wave_cells - 1; i++)
    {
        size_t x = i;
        size_t y = i + 1;
        self->cons[x].rho -= g_wave_dt_s / g_wave_dx_m * (self->flux[y].rho - self->flux[x].rho);
        self->cons[x].mom -= g_wave_dt_s / g_wave_dx_m * (self->flux[y].mom - self->flux[x].mom);
        self->cons[x].en  -= g_wave_dt_s / g_wave_dx_m * (self->flux[y].en  - self->flux[x].en);
        self->prim[x] = cons_to_prim(self->cons[x]);
    }
}

static void
set_hllc_wave_cell(struct wave_hllc_s* self, size_t index, struct wave_prim_s prim)
{
    self->prim[index] = prim;
    self->cons[index] = prim_to_cons(prim);
}

static void
step_hllc_wave(struct wave_hllc_s* self, struct wave_prim_s in, struct wave_prim_s out)
{
    if(in.u == 0.0)
    {
        in = self->prim[1];
        in.u = 0;
    }
    for(size_t i = 0; i < g_wave_substeps; i++)
    {
        set_hllc_wave_cell(self, 0, in);
        set_hllc_wave_cell(self, g_wave_cells - 1, out);
        compute_wave_flux(self);
        update_wave_state(self);
    }
}

static double
sample_hllc_wave(struct wave_hllc_s* self)
{
    size_t index = g_wave_cells * g_wave_mic_position_ratio;
    return self->prim[index].p;
}

static void
clear_wave_buffer()
{
    clear(g_wave_buffer_pa);
}

static void
add_to_wave_buffer(size_t wave_index)
{
    for(size_t i = 0; i < g_synth_buffer_size; i++)
    {
        g_wave_buffer_pa[i] += g_waves[wave_index].data.wave_sub_buffer_pa[i];
    }
}

static void
reset_hllc_wave_cells(struct wave_hllc_s* self)
{
    for(size_t i = 0; i < g_wave_cells; i++)
    {
        set_hllc_wave_cell(self, i, g_ambient_wave_cell);
    }
}

static void
reset_all_waves()
{
    for(size_t i = 0; i < g_wave_max_waves; i++)
    {
        struct wave_s* wave = &g_waves[i];
        for(size_t j = 0; j < g_synth_buffer_size; j++)
        {
            wave->data.buffer0[j] = g_ambient_wave_cell;
        }
        reset_hllc_wave_cells(&wave->hllc);
    }
}

static void
flip_wave(size_t wave_index)
{
    struct wave_s* self = &g_waves[wave_index];
    for(size_t i = 0; i < g_synth_buffer_size; i++)
    {
        self->data.buffer1[i] = self->data.buffer0[i];
    }
    self->data.index = 0;
}

static void
batch_step_wave(size_t wave_index)
{
    struct wave_s* self = &g_waves[wave_index];
    for(size_t i = 0; i < g_synth_buffer_size; i++)
    {
#if 1
        step_hllc_wave(&self->hllc, self->data.buffer1[i], g_ambient_wave_cell);
        self->data.wave_sub_buffer_pa[i] = sample_hllc_wave(&self->hllc);
#else
        self->data.wave_sub_buffer_pa[i] = self->data.buffer1[i].p;
#endif
    }
}

static void
stage_wave(size_t wave_index, struct wave_prim_s prim)
{
    struct wave_s* self = &g_waves[wave_index];
    self->data.buffer0[self->data.index++] = prim;
}

static void
step_wave(size_t wave_index, struct wave_prim_s prim)
{
    struct wave_s* wave = &g_waves[wave_index];
    step_hllc_wave(&wave->hllc, prim, g_ambient_wave_cell);
}
