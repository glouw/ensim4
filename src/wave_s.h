/*
 * One dimensional (pipe) computational fluid dynamics.
 */

constexpr size_t g_wave_cells = 128;
constexpr size_t g_wave_signal_cell_index = 0;
constexpr size_t g_wave_first_interior_cell_index = 1;
constexpr size_t g_wave_last_interior_cell_index = g_wave_cells - 2;
constexpr size_t g_wave_ambient_cell_index = g_wave_cells - 1;

/* 0                   1                             cells - 2                  cells - 1
 * +-------------+     +---------------------+       +--------------------+     +--------------+
 * | signal_cell | <-> | first_interior_cell | <...> | last_interior_cell | <-> | ambient_cell |
 * +-------------+     +---------------------+       +--------------------+     +--------------+
 *
 * Boundary equations always assume subsonic flow.
 *
 */

constexpr size_t g_wave_substeps = 8;
constexpr size_t g_flux_cells = g_wave_cells + 1;
constexpr size_t g_wave_max_waves = 4;
constexpr size_t g_wave_sample_rate_hz = g_std_audio_sample_rate_hz * g_wave_substeps;
constexpr double g_wave_gamma = 1.31;
constexpr double g_wave_dt_s = 1.0 / g_wave_sample_rate_hz;

struct wave_prim_s
{
    double r; // static_density_kg_per_m3
    double u; // velocity_m_per_s
    double p; // static_pressure_pa
};

struct wave_cons_s
{
    double r; // static_density_kg_per_m3
    double m; // momentum_density_kg_per_m2_s
    double e; // total_energy_density_j_per_m_3
};

struct wave_flux_s
{
    double r; // mass_flux_kg_per_m2_s
    double m; // momentum_flux_n_per_m2
    double e; // energy_flux_w_per_m2
};

struct wave_solver_s
{
    struct wave_prim_s prim[g_wave_cells];
    struct wave_cons_s cons[g_wave_cells];
    struct wave_flux_s flux[g_flux_cells];
    struct lowpass_filter_3_s u_filter;
    double velocity_low_pass_cutoff_frequency_hz;
    double gradient_s_per_m;
    double mic_position_ratio;
    double max_wave_speed_m_per_s;
    double pipe_length_m;
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
    struct wave_solver_s solver;
}
static g_wave_table[g_wave_max_waves] = {};

static double g_wave_buffer_pa[g_synth_buffer_size] = {};

constexpr struct wave_prim_s g_wave_ambient_cell = {
    .r = g_gas_ambient_static_density_kg_per_m3,
    .u = 0.0,
    .p = g_gas_ambient_static_pressure_pa,
};

static struct wave_cons_s
prim_to_cons(struct wave_prim_s self)
{
    /*       p      1         2
     * e = ----- + --- * r * u
     *     y - 1    2
     */
    return (struct wave_cons_s) {
        .r = self.r,
        .m = self.r * self.u,
        .e = self.p / (g_wave_gamma - 1.0) + 0.5 * self.r * self.u * self.u,
    };
}

static struct wave_prim_s
cons_to_prim(struct wave_cons_s self)
{
    /*                  2
     *           1     m
     * P = (e - --- * ---) * (y - 1)
     *           2     r
     */
    double u = self.m / self.r;
    return (struct wave_prim_s) {
        .r = self.r,
        .u = self.m / self.r,
        .p = (self.e - 0.5 * self.m * u) * (g_wave_gamma - 1.0),
    };
}

static struct wave_flux_s
calc_solver_flux(struct wave_prim_s ql, struct wave_prim_s qr)
{
    struct wave_cons_s ul = prim_to_cons(ql);
    struct wave_cons_s ur = prim_to_cons(qr);
    double cl = sqrt(g_wave_gamma * ql.p / ql.r);
    double cr = sqrt(g_wave_gamma * qr.p / qr.r);
    struct wave_flux_s fl = { .r = ul.m, .m = ul.m * ql.u + ql.p, .e = (ul.e + ql.p) * ql.u };
    struct wave_flux_s fr = { .r = ur.m, .m = ur.m * qr.u + qr.p, .e = (ur.e + qr.p) * qr.u };
    double a = fmax(fabs(ql.u) + cl, fabs(qr.u) + cr);
    /*
     *       1               1
     * FC = --- (FL + FR) - --- a * (UR - UL)
     *       2               2
     */
    return (struct wave_flux_s) {
        .r = 0.5 * (fl.r + fr.r) - 0.5 * a * (ur.r - ul.r),
        .m = 0.5 * (fl.m + fr.m) - 0.5 * a * (ur.m - ul.m),
        .e = 0.5 * (fl.e + fr.e) - 0.5 * a * (ur.e - ul.e),
    };
}

static void
compute_wave_flux(struct wave_solver_s* self)
{
    size_t l = g_wave_signal_cell_index;
    size_t r = g_wave_cells;
    size_t z = g_wave_ambient_cell_index;
    self->flux[l] = calc_solver_flux(self->prim[l], self->prim[l]);
    self->flux[r] = calc_solver_flux(self->prim[z], self->prim[z]);
    for(size_t i = 1; i < g_wave_cells; i++)
    {
        size_t x = i - 1;
        size_t y = i;
        self->flux[y] = calc_solver_flux(self->prim[x], self->prim[y]);
    }
}

static void
update_wave_state(struct wave_solver_s* self)
{
    for(size_t i = 1; i < g_wave_ambient_cell_index; i++)
    {
        size_t x = i;
        size_t y = i + 1;
        self->cons[x].r -= self->gradient_s_per_m * (self->flux[y].r - self->flux[x].r);
        self->cons[x].m -= self->gradient_s_per_m * (self->flux[y].m - self->flux[x].m);
        self->cons[x].e -= self->gradient_s_per_m * (self->flux[y].e - self->flux[x].e);
        self->prim[x] = cons_to_prim(self->cons[x]);
    }
}

static void
set_solver_wave_cell(struct wave_solver_s* self, size_t index, struct wave_prim_s prim)
{
    self->prim[index] = prim;
    self->cons[index] = prim_to_cons(prim);
}

static struct wave_prim_s
calc_signal_cell(struct wave_solver_s* self, struct wave_prim_s signal)
{
    return (struct wave_prim_s) {
        .r = signal.r,
        .u = filter_lowpass_3(&self->u_filter, self->velocity_low_pass_cutoff_frequency_hz, signal.u),
        .p = signal.p
    };
}

static struct wave_prim_s
calc_ambient_cell(struct wave_solver_s* self)
{
    struct wave_prim_s signal = g_wave_ambient_cell;
    struct wave_prim_s last_interior_cell = self->prim[g_wave_last_interior_cell_index];
    return (struct wave_prim_s) {
        .r = last_interior_cell.r * pow(signal.p / last_interior_cell.p, 1.0 / g_wave_gamma),
        .u = last_interior_cell.u,
        .p = signal.p
    };
}

static void
step_solver_wave(struct wave_solver_s* self, struct wave_prim_s signal)
{
    for(size_t i = 0; i < g_wave_substeps; i++)
    {
        struct wave_prim_s signal_cell = calc_signal_cell(self, signal);
        struct wave_prim_s ambient_cell = calc_ambient_cell(self);
        set_solver_wave_cell(self, g_wave_signal_cell_index, signal_cell);
        set_solver_wave_cell(self, g_wave_ambient_cell_index, ambient_cell);
        compute_wave_flux(self);
        update_wave_state(self);
    }
}

static double
sample_solver_wave(struct wave_solver_s* self)
{
    size_t index = g_wave_ambient_cell_index * self->mic_position_ratio;
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
        g_wave_buffer_pa[i] += g_wave_table[wave_index].data.wave_sub_buffer_pa[i];
    }
}

static void
reset_solver_wave_cells(struct wave_solver_s* self)
{
    for(size_t i = 0; i < g_wave_cells; i++)
    {
        set_solver_wave_cell(self, i, g_wave_ambient_cell);
    }
}

static void
reset_all_waves()
{
    for(size_t i = 0; i < g_wave_max_waves; i++)
    {
        struct wave_s* wave = &g_wave_table[i];
        for(size_t j = 0; j < g_synth_buffer_size; j++)
        {
            wave->data.buffer0[j] = g_wave_ambient_cell;
        }
        reset_solver_wave_cells(&wave->solver);
    }
}

static void
flip_wave(size_t wave_index)
{
    struct wave_s* self = &g_wave_table[wave_index];
    for(size_t i = 0; i < g_synth_buffer_size; i++)
    {
        self->data.buffer1[i] = self->data.buffer0[i];
    }
    self->data.index = 0;
}

static void
batch_wave(
    size_t wave_index,
    bool use_cfd,
    double pipe_length_m,
    double mic_position_ratio,
    double velocity_low_pass_cutoff_frequency_hz)
{
    struct wave_s* self = &g_wave_table[wave_index];
    double wave_dx_m = pipe_length_m / g_wave_cells;
    self->solver.max_wave_speed_m_per_s = wave_dx_m / g_wave_dt_s;
    self->solver.gradient_s_per_m = g_wave_dt_s / wave_dx_m;
    self->solver.pipe_length_m = pipe_length_m;
    self->solver.mic_position_ratio = mic_position_ratio;
    self->solver.velocity_low_pass_cutoff_frequency_hz = velocity_low_pass_cutoff_frequency_hz;
    for(size_t i = 0; i < g_synth_buffer_size; i++)
    {
        if(use_cfd)
        {
            step_solver_wave(&self->solver, self->data.buffer1[i]);
            self->data.wave_sub_buffer_pa[i] = sample_solver_wave(&self->solver);
        }
        else
        {
            self->data.wave_sub_buffer_pa[i] = self->data.buffer1[i].p;
        }
    }
}

static void
stage_wave(size_t wave_index, struct wave_prim_s prim)
{
    struct wave_s* self = &g_wave_table[wave_index];
    self->data.buffer0[self->data.index++] = prim;
}
