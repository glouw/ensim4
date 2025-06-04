static constexpr double chamber_total_pressure_hysteresis_pa = 1.0;

struct chamber_s
{
    struct gas_s gas;
    double volume_m3;
    double nozzle_max_flow_area_m2;
    double nozzle_open_ratio;
    size_t flow_cycles;
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
    return calc_static_pressure_pa(self) - gas_ambient_static_pressure_pa;
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
    if(Pt0 - Pt1 < chamber_total_pressure_hysteresis_pa)
    {
        return 0.0;
    }
    else
    {
        double y = calc_mixed_gamma(&self->gas);
        double Ps = calc_static_pressure_pa(other);
        double M = sqrt((2.0 / (y - 1.0)) * (pow(Pt0 / Ps, (y - 1.0) / y) - 1.0));
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

/*                   y - 1
 *                   -----
 *                     y
 *               Ps2
 * Ts2 = Ts1 * (-----)
 *               Ps1
 *
 */

static double
calc_new_adiabatic_static_temperature_from_pressure_delta_k(const struct chamber_s* self, double old_static_pressure_pa)
{
    double Ps1 = old_static_pressure_pa;
    double Ps2 = calc_static_pressure_pa(self);
    double y = calc_mixed_gamma(&self->gas);
    return self->gas.static_temperature_k * pow(Ps2 / Ps1, (y - 1.0) / y);
}

/*                   y - 1
 *               V1
 * Ts2 = Ts1 * (----)
 *               V2
 *
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
    double old_static_pressure_pa = calc_static_pressure_pa(self);
    self->gas.mass_kg -= mail->mass_kg;
    self->gas.momentum_kg_m_per_s -= mail->momentum_kg_m_per_s;
    self->gas.static_temperature_k = calc_new_adiabatic_static_temperature_from_pressure_delta_k(self, old_static_pressure_pa);
}

static void
normalize_chamber(struct chamber_s* self)
{
    self->gas = gas_ambient_air;
    self->gas.mass_kg = calc_mass_at_kg(self, gas_ambient_static_pressure_pa);
}
