struct chamber_t
{
    struct gas_t gas;
    double volume_m3;
    double nozzle_max_flow_area_m2;
    double nozzle_open_ratio;
};

/*
 *      m * R * Ts
 * Ps = ----------
 *          V
 */

static double
calc_static_pressure_pa(const struct chamber_t* self)
{
    double m = self->gas.mass_kg;
    double Rs = calc_specific_gas_constant_j_per_kg_k(&self->gas);
    double Ts = self->gas.static_temperature_k;
    double V = self->volume_m3;
    return m * Rs * Ts / V;
}

/*                                y
 *                              -----
 *                (y - 1)    2 (y - 1)
 * Pt = Ps * (1 + ------- * M )
 *                   2
 */

static double
calc_total_pressure_pa(const struct chamber_t* self)
{
    double y = calc_mixed_gamma(&self->gas);
    double Ps = calc_static_pressure_pa(self);
    double M = calc_bulk_mach(&self->gas);
    return Ps * pow(1.0 + (y - 1.0) / 2.0 * pow(M, 2.0), y / (y - 1.0));
}

/*
 *                (y - 1)    2
 * Tt = Ts * (1 + ------- * M )
 *                   2
 */

static double
calc_total_temperature_k(const struct chamber_t* self)
{
    double y = calc_mixed_gamma(&self->gas);
    double Ts = self->gas.static_temperature_k;
    double M = calc_bulk_mach(&self->gas);
    return Ts * (1.0 + (y - 1.0) / 2.0 * pow(M, 2.0));
}

/*             __________________________
 *            /
 *           /                (y - 1)
 *          /                 -------
 *         /    2         Pt     y
 * M = _  /  (-----) * [(----)       - 1]
 *      \/    y - 1       Ps
 */

static double
calc_nozzle_mach(const struct chamber_t* self, const struct chamber_t* other)
{
    double y = calc_mixed_gamma(&self->gas);
    double Ps = calc_static_pressure_pa(other);
    double Pt0 = calc_total_pressure_pa(self);
    double Pt1 = calc_total_pressure_pa(other);
    double total_pressure_hysteresis_pa = 10.0;
    if(Pt0 - Pt1 < total_pressure_hysteresis_pa)
    {
        return 0.0;
    }
    else
    {
        double M = sqrt((2.0 / (y - 1.0)) * (pow(Pt0 / Ps, (y - 1.0) / y) - 1.0));
        return clamp(M, 0.0, 1.0);
    }
}

/*
 *                     ____
 * .    A * Pt        / y                    M
 * m = -------- * _  / ---- * (-------------------------------)
 *    _  ____      \/   Rs                            y + 1
 *     \/ Tt                                       -----------
 *                                                 2 * (y - 1)
 *                                   (y - 1)    2
 *                             [ 1 + ------- * M ]
 *                                      2
 */

static double
calc_nozzle_mass_flow_rate_kg_per_s(const struct chamber_t* self, const struct chamber_t* other, double nozzle_flow_area_m2)
{
    double y = calc_mixed_gamma(&self->gas);
    double M = calc_nozzle_mach(self, other);
    double Rs = calc_specific_gas_constant_j_per_kg_k(&self->gas);
    double Tt = calc_total_temperature_k(self);
    double Pt = calc_total_pressure_pa(self);
    double A = nozzle_flow_area_m2;
    return A * Pt / sqrt(Tt) * sqrt(y / Rs) * M / pow(1.0 + (y - 1.0) / 2.0 * pow(M, 2.0), (y + 1.0) / (2.0 * (y - 1.0)));
}

/*               _________________________
 *              /
 *             /                   (y - 1)
 *            /                    -------
 *           /                 Ps     y
 * u = M _  /  y * Rs * Tt * (----)
 *        \/                   Pt
 */

static double
calc_nozzle_flow_velocity_m_per_s(const struct chamber_t* self, const struct chamber_t* other)
{
    double y = calc_mixed_gamma(&self->gas);
    double M = calc_nozzle_mach(self, other);
    double Rs = calc_specific_gas_constant_j_per_kg_k(&self->gas);
    double Tt = calc_total_temperature_k(self);
    double Ps = calc_static_pressure_pa(other);
    double Pt = calc_total_pressure_pa(self);
    return M * sqrt(y * Rs * Tt * pow(Ps / Pt, (y - 1.0) / y));
}

/*                   y - 1
 *                   -----
 *                     y
 *               Ps2
 * Ts2 = Ts1 * (-----)
 *               Ps1
 *
 */

static double
calc_new_adiabatic_static_temperature_k(const struct chamber_t* self, double old_static_pressure_pa)
{
    double Ps1 = old_static_pressure_pa;
    double Ps2 = calc_static_pressure_pa(self);
    double y = calc_mixed_gamma(&self->gas);
    return pow(Ps2 / Ps1, (y - 1.0) / y);
}

static double
calc_nozzle_flow_area_m2(const struct chamber_t* self)
{
    return self->nozzle_max_flow_area_m2 * self->nozzle_open_ratio;
}

static void
remove_gas(struct chamber_t* self, const struct gas_t* mail)
{
    double old_static_pressure_pa = calc_static_pressure_pa(self);
    self->gas.mass_kg -= mail->mass_kg;
    self->gas.momentum_kg_m_per_s -= mail->momentum_kg_m_per_s;
    self->gas.static_temperature_k *= calc_new_adiabatic_static_temperature_k(self, old_static_pressure_pa);
}

static void
flow(struct chamber_t* x, struct chamber_t* y)
{
    double nozzle_flow_area_m2 = calc_nozzle_flow_area_m2(x);
    if(nozzle_flow_area_m2 > 0.0)
    {
        /* always flows from chamber x to chamber y */
        if(calc_total_pressure_pa(x) < calc_total_pressure_pa(y))
        {
            struct chamber_t* copy = x;
            x = y;
            y = copy;
        }
        double nozzle_flow_velocity_m_per_s = calc_nozzle_flow_velocity_m_per_s(x, y);
        double nozzle_mass_flow_rate_kg_per_s = calc_nozzle_mass_flow_rate_kg_per_s(x, y, nozzle_flow_area_m2);
        double mass_flowed_kg = nozzle_mass_flow_rate_kg_per_s * dt_s;
        double momentum_transferred_kg = mass_flowed_kg * nozzle_flow_velocity_m_per_s;
        struct gas_t mail = {
            .mol_ratio_c8h18 = x->gas.mol_ratio_c8h18,
            .mol_ratio_o2 = x->gas.mol_ratio_o2,
            .mol_ratio_n2 = x->gas.mol_ratio_n2,
            .mol_ratio_ar = x->gas.mol_ratio_ar,
            .mol_ratio_co2 = x->gas.mol_ratio_co2,
            .mol_ratio_h2o = x->gas.mol_ratio_h2o,
            .static_temperature_k = x->gas.static_temperature_k,
            .mass_kg = mass_flowed_kg,
            .momentum_kg_m_per_s = momentum_transferred_kg,
        };
        remove_gas(x, &mail);
        mix_in_gas(&y->gas, &mail);
        clamp_momentum(&x->gas);
        clamp_momentum(&y->gas);
    }
}
