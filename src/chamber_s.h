static constexpr double g_chamber_total_pressure_deadband_pa = 500.0;
static constexpr double g_chamber_c8h18_heat_of_combustion_j_per_mol = 5.47e6;

struct chamber_s
{
    struct gas_s gas;
    double volume_m3;
    double nozzle_max_flow_area_m2;
    double nozzle_open_ratio;
    size_t flow_cycles;
    bool should_panic;
};

/*
 *      m * R * Ts
 * Ps = ----------
 *          V
 */

static double
calc_static_pressure_pa(const struct chamber_s* self)
{
    double m = self->gas.mass_kg;
    double Rs = calc_specific_gas_constant_j_per_kg_k(&self->gas);
    double Ts = self->gas.static_temperature_k;
    double V = self->volume_m3;
    return m * Rs * Ts / V;
}

static double
calc_static_gauge_pressure_pa(const struct chamber_s* self)
{
    return calc_static_pressure_pa(self) - g_gas_ambient_static_pressure_pa;
}

/*
 *     Ps * V
 * m = -------
 *     Rs * Ts
 */

static double
calc_mass_at_kg(const struct chamber_s* self, double static_pressure_pa)
{
    double Ps = static_pressure_pa;
    double V = self->volume_m3;
    double Rs = calc_specific_gas_constant_j_per_kg_k(&self->gas);
    double Ts = self->gas.static_temperature_k;
    return Ps * V / (Rs * Ts);
}

/*                                y
 *                              -----
 *                (y - 1)    2 (y - 1)
 * Pt = Ps * (1 + ------- * M )
 *                   2
 */

static double
calc_total_pressure_pa(const struct chamber_s* self)
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
calc_total_temperature_k(const struct chamber_s* self)
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
calc_nozzle_mach(const struct chamber_s* self, const struct chamber_s* other)
{
    double Pt0 = calc_total_pressure_pa(self);
    double Pt1 = calc_total_pressure_pa(other);
    if(Pt0 - Pt1 < g_chamber_total_pressure_deadband_pa)
    {
        return 0.0;
    }
    else
    {
        double y = calc_mixed_gamma(&self->gas);
        double Ps = calc_static_pressure_pa(other);
        double M = sqrt((2.0 / (y - 1.0)) * (pow(Pt0 / Ps, (y - 1.0) / y) - 1.0));

        /* Assumes straight nozzle or convergent nozzle,
         * preventing supersonic mach numbers.
         */
        return clamp(M, 0.0, 1.0);
    }
}

/*
 *                     ____
 * .    A * Pt        / y                    M
 * m = -------- * _  / ---- * (-------------------------------)
 *     _  ____     \/   Rs                            y + 1
 *      \/ Tt                                      -----------
 *                                                 2 * (y - 1)
 *                                   (y - 1)    2
 *                             [ 1 + ------- * M ]
 *                                      2
 */

static double
calc_nozzle_mass_flow_rate_kg_per_s(const struct chamber_s* self, double nozzle_flow_area_m2, double nozzle_mach)
{
    double y = calc_mixed_gamma(&self->gas);
    double M = nozzle_mach;
    double Rs = calc_specific_gas_constant_j_per_kg_k(&self->gas);
    double Tt = calc_total_temperature_k(self);
    double Pt = calc_total_pressure_pa(self);
    double A = nozzle_flow_area_m2;
    return A * Pt / sqrt(Tt) * sqrt(y / Rs) * M / pow(1.0 + (y - 1.0) / 2.0 * pow(M, 2.0), (y + 1.0) / (2.0 * (y - 1.0)));
}

/*               ______________
 *              /
 *             /  y * Rs * Tt
 *            / --------------
 *           /       y - 1   2
 * u = M _  /   1 + (-----) M
 *        \/           2
 */

static double
calc_nozzle_flow_velocity_m_per_s(const struct chamber_s* self, double nozzle_mach)
{
    double y = calc_mixed_gamma(&self->gas);
    double M = nozzle_mach;
    double Rs = calc_specific_gas_constant_j_per_kg_k(&self->gas);
    double Tt = calc_total_temperature_k(self);
    return M * sqrt(y * Rs * Tt / (1.0 + (y - 1.0) / 2.0 * pow(M, 2.0)));
}

/*
 *      u
 * a = ---
 *      M
 */

static double
calc_nozzle_speed_of_sound_m_per_s(const struct chamber_s* self, double nozzle_mach, double nozzle_flow_velocity_m_per_s)
{
    double u = nozzle_flow_velocity_m_per_s;
    double M = nozzle_mach;
    if(M == 0.0)
    {
        return calc_bulk_speed_of_sound_m_per_s(&self->gas);
    }
    else
    {
        return u / M;
    }
}

static double
calc_nozzle_flow_area_m2(const struct chamber_s* self)
{
    return self->nozzle_max_flow_area_m2 * self->nozzle_open_ratio;
}

/*         .
 *         m
 * ps = -------
 *      (A * u)
 */

static double
calc_nozzle_static_density_kg_per_m3(double nozzle_mass_flow_rate_kg_per_s, double nozzle_flow_area_m2, double nozzle_flow_velocity_m_per_s)
{
    return nozzle_mass_flow_rate_kg_per_s / (nozzle_flow_area_m2 * nozzle_flow_velocity_m_per_s);
}

/*                   y - 1
 *               V1
 * Ts2 = Ts1 * (----)
 *               V2
 */

static double
calc_new_adiabatic_static_temperature_from_volume_delta_k(const struct chamber_s* self, double old_volume_m3)
{
    double V1 = old_volume_m3;
    double V2 = self->volume_m3;
    double y = calc_mixed_gamma(&self->gas);
    return self->gas.static_temperature_k * pow(V1 / V2, y - 1.0);
}

static void
remove_gas(struct chamber_s* self, const struct gas_s* mail)
{
    self->gas.mass_kg -= mail->mass_kg;
    if(self->gas.mass_kg < 0.0)
    {
        self->should_panic = true;
        g_std_panic_message = "negative chamber mass detected";
    }
    add_momentum(&self->gas, -mail->momentum_kg_m_per_s);
}

static void
normalize_chamber(struct chamber_s* self)
{
    self->gas = g_gas_ambient_air;
    self->gas.mass_kg = calc_mass_at_kg(self, g_gas_ambient_static_pressure_pa);
}

/* Fuel chambers are treated as a gas, atomized into fine droplets.
 * The fuel remains dispersed and does not re-liquefy.
 * Mass is doubled and temperature s raised to simulate a high-pressure fuel injection source.
 */

static void
normalize_injection_chamber(struct chamber_s* self)
{
    self->gas = g_gas_ambient_atomized_c8h18_fuel;
    self->gas.mass_kg = calc_mass_at_kg(self, g_gas_ambient_static_pressure_pa);
    self->gas.mass_kg *= 2.0;
    self->gas.static_temperature_k += 30.0;
}

/*
 *  c8h18 + 12.5 o2 -> 8 co2 + 9 h2o
 */

static void
combust_c8h18(struct chamber_s* self, double fraction)
{
    double mol_ratio_c8h18 = self->gas.mol_ratio_c8h18 * fraction;
    double mol_ratio_o2 = 12.5 * mol_ratio_c8h18;
    if(mol_ratio_o2 > self->gas.mol_ratio_o2)
    {
        mol_ratio_c8h18 *= self->gas.mol_ratio_o2 / mol_ratio_o2;
        mol_ratio_o2 = 12.5 * mol_ratio_c8h18;
    }
    self->gas.mol_ratio_c8h18 -= mol_ratio_c8h18;
    self->gas.mol_ratio_o2 -= mol_ratio_o2;
    self->gas.mol_ratio_co2 += 8.0 * mol_ratio_c8h18;
    self->gas.mol_ratio_h2o += 9.0 * mol_ratio_c8h18;
    double mol_ratio
        = self->gas.mol_ratio_n2
        + self->gas.mol_ratio_o2
        + self->gas.mol_ratio_ar
        + self->gas.mol_ratio_c8h18
        + self->gas.mol_ratio_co2
        + self->gas.mol_ratio_h2o;
    self->gas.mol_ratio_n2 /= mol_ratio;
    self->gas.mol_ratio_o2 /= mol_ratio;
    self->gas.mol_ratio_ar /= mol_ratio;
    self->gas.mol_ratio_c8h18 /= mol_ratio;
    self->gas.mol_ratio_co2 /= mol_ratio;
    self->gas.mol_ratio_h2o /= mol_ratio;
    double energy_j_per_mol = mol_ratio_c8h18 * g_chamber_c8h18_heat_of_combustion_j_per_mol;
    self->gas.static_temperature_k += energy_j_per_mol / calc_mixed_cv_j_per_mol_k(&self->gas);
}
