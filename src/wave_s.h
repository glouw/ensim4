static constexpr size_t g_wave_cells = 64;
static constexpr size_t g_wave_signal_cell_index = 0;
static constexpr size_t g_wave_first_interior_cell_index = 1;
static constexpr size_t g_wave_last_interior_cell_index = g_wave_cells - 2;
static constexpr size_t g_wave_ambient_cell_index = g_wave_cells - 1;
static constexpr size_t g_wave_substeps = 6;
static constexpr size_t g_wave_max_waves = 16;
static constexpr size_t g_wave_sample_rate_hz = g_std_audio_sample_rate_hz * g_wave_substeps;
static constexpr double g_wave_gamma = 1.31;
static constexpr double g_wave_dt_s = 1.0 / g_wave_sample_rate_hz;
static constexpr double g_wave_mic_position_ratio = 1.00;
static constexpr double g_wave_pipe_length_m = 0.6;
static constexpr double g_wave_dx_m = g_wave_pipe_length_m / g_wave_cells;
static constexpr double g_wave_max_wave_speed_m_per_s = g_wave_dx_m / g_wave_dt_s;

/*
 * Units and longform names were ommited from variable names to simplify readability.
 */

struct wave_prim_s
{
    /*
     * static_density_kg_per_m3
     */
    double rho;

    /*
     * velocity_m_per_s
     */
    double u;

    /*
     * static_pressure_pa
     */
    double p;
};

struct wave_cons_s
{
    /*
     * static_density_kg_per_m3
     */
    double rho;

    /*
     * momentum_density_kg_per_m2_s
     */
    double mom;

    /*
     * total_energy_density_j_per_m_3
     */
    double en;
};

struct wave_flux_s
{
    /*
     * mass_flux_kg_per_m2_s
     */
    double rho;

    /*
     * momentum_flux_n_per_m2
     */
    double mom;

    /*
     * energy_flux_w_per_m2
     */
    double en;
};

struct wave_hllc_s
{
    struct wave_prim_s prim[g_wave_cells];
    struct wave_cons_s cons[g_wave_cells];
    struct wave_flux_s flux[g_wave_cells + 1];
    bool should_panic;
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
g_waves[g_wave_max_waves] = {};

static double g_wave_buffer_pa[g_synth_buffer_size] = {};

static constexpr struct wave_prim_s g_wave_ambient_cell = {
    .rho = g_gas_ambient_static_density_kg_per_m3,
    .u = 0.0,
    .p = g_gas_ambient_static_pressure_pa,
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

/*
 * Standard Euler Flux.
 */

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

/*
 *  .       S - u
 * p  = p * ------
 *               .
 *          S - S
 */

static double
calc_rho_star(
    struct wave_cons_s u,
    struct wave_prim_s q,
    double s,
    double s_star)
{
    return u.rho * (s - q.u) / (s - s_star);
}

/*
 *
 *  .    .     E      .          .       p
 * E  = p  * (--- + (S  - u) * (S  + -----------))
 *             p                     p * (S - u)
 *
 *
 */

static double
calc_en_star(
    double rho_star,
    struct wave_cons_s u,
    struct wave_prim_s q,
    double s,
    double s_star)
{
    return rho_star * ((u.en / u.rho) + (s_star - q.u) * (s_star + q.p / (u.rho * (s - q.u))));
}

/*
 *  .   PsR - PsL + pL * Ul * (SL - uL) - pR * uR * (SR - uR)
 * S  = -----------------------------------------------------
 *              pL * (SL - uL) - pR * (SR - uR)
 *
 */

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
calc_hllc_flux_component(
    struct wave_cons_s u,
    struct wave_flux_s f_base,
    struct wave_prim_s q,
    double s,
    double s_star)
{
    double rho_star = calc_rho_star(u, q, s, s_star);
    double en_star = calc_en_star(rho_star, u, q, s, s_star);
    return (struct wave_flux_s) {
        .rho = f_base.rho + s * (rho_star - u.rho),
        .mom = f_base.mom + s * (rho_star * s_star - u.mom),
        .en = f_base.en + s * (en_star - u.en),
    };
}

static struct wave_flux_s
calc_hllc_flux(struct wave_hllc_s* self, struct wave_prim_s ql, struct wave_prim_s qr)
{
    double cl = calc_cell_speed_of_sound(ql);
    double cr = calc_cell_speed_of_sound(qr);
    double sl = fmin(ql.u - cl, qr.u - cr);
    double sr = fmax(ql.u + cl, qr.u + cr);
    if(fabs(sl) > g_wave_max_wave_speed_m_per_s)
    {
        self->should_panic = true;
        g_std_panic_message = "hllc left wave exceeded max speed";
    }
    if(fabs(sr) > g_wave_max_wave_speed_m_per_s)
    {
        self->should_panic = true;
        g_std_panic_message = "hllc right wave exceeded max speed";
    }
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
    size_t l = g_wave_signal_cell_index;
    size_t r = g_wave_cells;
    size_t z = g_wave_ambient_cell_index;
    self->flux[l] = calc_hllc_flux(self, self->prim[l], self->prim[l]);
    self->flux[r] = calc_hllc_flux(self, self->prim[z], self->prim[z]);
    for(size_t i = g_wave_first_interior_cell_index; i < g_wave_cells; i++)
    {
        size_t x = i - 1;
        size_t y = i;
        self->flux[y] = calc_hllc_flux(self, self->prim[x], self->prim[y]);
    }
}

static void
update_wave_state(struct wave_hllc_s* self)
{
    for(size_t i = g_wave_first_interior_cell_index; i < g_wave_ambient_cell_index; i++)
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
step_hllc_wave(struct wave_hllc_s* self, struct wave_prim_s signal_cell)
{
    if(signal_cell.u == 0.0)
    {
        signal_cell = self->prim[g_wave_first_interior_cell_index];
        signal_cell.u = 0.0;
    }
    for(size_t i = 0; i < g_wave_substeps; i++)
    {
        struct wave_prim_s last_interior_cell = self->prim[g_wave_last_interior_cell_index];
        /*
         * Assume subsonic exit boundary conditions.
         */
        struct wave_prim_s ambient_cell = {
            .rho = last_interior_cell.rho,
            .u = last_interior_cell.u,
            .p = g_wave_ambient_cell.p,
        };
        set_hllc_wave_cell(self, g_wave_signal_cell_index, signal_cell);
        set_hllc_wave_cell(self, g_wave_ambient_cell_index, ambient_cell);
        compute_wave_flux(self);
        update_wave_state(self);
    }
}

static double
sample_hllc_wave(struct wave_hllc_s* self)
{
    size_t index = g_wave_last_interior_cell_index * g_wave_mic_position_ratio;
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
        set_hllc_wave_cell(self, i, g_wave_ambient_cell);
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
            wave->data.buffer0[j] = g_wave_ambient_cell;
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
batch_step_wave(size_t wave_index, bool use_cfd)
{
    struct wave_s* self = &g_waves[wave_index];
    for(size_t i = 0; i < g_synth_buffer_size; i++)
    {
        if(use_cfd)
        {
            step_hllc_wave(&self->hllc, self->data.buffer1[i]);
            self->data.wave_sub_buffer_pa[i] = sample_hllc_wave(&self->hllc);
        }
        else /* just pass through */
        {
            self->data.wave_sub_buffer_pa[i] = self->data.buffer1[i].p;
        }
    }
}

static void
stage_wave(size_t wave_index, struct wave_prim_s prim)
{
    struct wave_s* self = &g_waves[wave_index];
    self->data.buffer0[self->data.index++] = prim;
}
