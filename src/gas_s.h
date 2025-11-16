constexpr double g_gas_molar_mass_kg_per_mol_c8h18 = 0.1142285200;
constexpr double g_gas_molar_mass_kg_per_mol_o2 = 0.0319988000;
constexpr double g_gas_molar_mass_kg_per_mol_n2 = 0.0280134000;
constexpr double g_gas_molar_mass_kg_per_mol_ar = 0.0399480000;
constexpr double g_gas_molar_mass_kg_per_mol_co2 = 0.0440095000;
constexpr double g_gas_molar_mass_kg_per_mol_h2o = 0.0180152800;
constexpr double g_gas_ideal_mol_air_fuel_ratio = 59.5;
constexpr double g_gas_ambient_static_temperature_k = 300.0;
constexpr double g_gas_ambient_static_pressure_pa = 101325.0;
constexpr double g_gas_ambient_static_density_kg_per_m3 = 1.225;

struct gas_s
{
    double mol_ratio_n2;
    double mol_ratio_o2;
    double mol_ratio_ar;
    double mol_ratio_c8h18;
    double mol_ratio_co2;
    double mol_ratio_h2o;
    double static_temperature_k;
    double mass_kg;
    double momentum_kg_m_per_s;
};

constexpr struct gas_s g_gas_ambient_air = {
    .mol_ratio_n2 = 0.78,
    .mol_ratio_o2 = 0.21,
    .mol_ratio_ar = 0.01,
    .static_temperature_k = g_gas_ambient_static_temperature_k
};

constexpr struct gas_s g_gas_ambient_atomized_c8h18_fuel = {
    .mol_ratio_c8h18 = 1.0,
    .static_temperature_k = g_gas_ambient_static_temperature_k
};

static double
calc_mol_air_ratio(struct gas_s* self)
{
    return self->mol_ratio_n2
         + self->mol_ratio_o2
         + self->mol_ratio_ar;
}

static double
calc_mol_combusted_ratio(struct gas_s* self)
{
    return self->mol_ratio_co2
         + self->mol_ratio_h2o;
}

static double
calc_mol_air_fuel_ratio(struct gas_s* self)
{
    return calc_mol_air_ratio(self) / self->mol_ratio_c8h18;
}

static double
calc_mixed_molar_mass_kg_per_mol(struct gas_s* self)
{
    return
        self->mol_ratio_n2 * g_gas_molar_mass_kg_per_mol_n2 +
        self->mol_ratio_o2 * g_gas_molar_mass_kg_per_mol_o2 +
        self->mol_ratio_ar * g_gas_molar_mass_kg_per_mol_ar +
        self->mol_ratio_c8h18 * g_gas_molar_mass_kg_per_mol_c8h18 +
        self->mol_ratio_co2 * g_gas_molar_mass_kg_per_mol_co2 +
        self->mol_ratio_h2o * g_gas_molar_mass_kg_per_mol_h2o;
}

static double
calc_mixed_cp_j_per_mol_k(struct gas_s* self)
{
    return
        self->mol_ratio_n2 * lookup_cp_n2_j_per_mol_k(self->static_temperature_k) +
        self->mol_ratio_o2 * lookup_cp_o2_j_per_mol_k(self->static_temperature_k) +
        self->mol_ratio_ar * lookup_cp_ar_j_per_mol_k(self->static_temperature_k) +
        self->mol_ratio_c8h18 * lookup_cp_c8h18_j_per_mol_k(self->static_temperature_k) +
        self->mol_ratio_co2 * lookup_cp_co2_j_per_mol_k(self->static_temperature_k) +
        self->mol_ratio_h2o * lookup_cp_h2o_j_per_mol_k(self->static_temperature_k);
}

static double
calc_mixed_cv_j_per_mol_k(struct gas_s* self)
{
    return calc_cv_j_per_mol_k(calc_mixed_cp_j_per_mol_k(self));
}

static double
calc_mixed_gamma(struct gas_s* self)
{
    return lookup_gamma(calc_mixed_cp_j_per_mol_k(self));
}

/*
 *      m
 * n = ---
 *      M
 */

static double
calc_moles(struct gas_s* self)
{
    double m = self->mass_kg;
    double M = calc_mixed_molar_mass_kg_per_mol(self);
    return m / M;
}

static double
calc_total_cp_j_per_k(struct gas_s* self)
{
    return calc_moles(self) * calc_mixed_cp_j_per_mol_k(self);
}

static double
calc_total_cv_j_per_k(struct gas_s* self)
{
    return calc_moles(self) * calc_mixed_cv_j_per_mol_k(self);
}

/*
 *       R
 * Rs = ---
 *       M
 */

static double
calc_specific_gas_constant_j_per_kg_k(struct gas_s* self)
{
    double R = g_gamma_universal_gas_constant_j_per_mol_k;
    double M = calc_mixed_molar_mass_kg_per_mol(self);
    return R / M;
}

/*
 *      p
 * u = ---
 *      m
 */

static double
calc_bulk_flow_velocity_m_per_s(struct gas_s* self)
{
    double p = self->momentum_kg_m_per_s;
    double m = self->mass_kg;
    return p / m;
}

/*          ___________
 *         /
 * a = _  / y * Rs * Ts
 *      \/
 */

static double
calc_bulk_speed_of_sound_m_per_s(struct gas_s* self)
{
    double y = calc_mixed_gamma(self);
    double Rs = calc_specific_gas_constant_j_per_kg_k(self);
    double Ts = self->static_temperature_k;
    return sqrt(y * Rs * Ts);
}

/*
 *      u
 * M = ---
 *      a
 */

static double
calc_bulk_mach(struct gas_s* self)
{
    double u = calc_bulk_flow_velocity_m_per_s(self);
    double a = calc_bulk_speed_of_sound_m_per_s(self);
    return u / a;
}

/*
 * p = m * c
 *
 */

static double
calc_max_bulk_momentum_kg_m_per_s(struct gas_s* self)
{
    double m = self->mass_kg;
    double a = calc_bulk_speed_of_sound_m_per_s(self);
    return m * a;
}

static void
clamp_momentum(struct gas_s* self)
{
    double max_bulk_momentum_kg_m_per_s = calc_max_bulk_momentum_kg_m_per_s(self);
    self->momentum_kg_m_per_s = clamp(self->momentum_kg_m_per_s, -max_bulk_momentum_kg_m_per_s, max_bulk_momentum_kg_m_per_s);
}
