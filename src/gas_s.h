constexpr double molar_mass_kg_per_mol_c8h18 = 0.1142285200;
constexpr double molar_mass_kg_per_mol_o2 = 0.0319988000;
constexpr double molar_mass_kg_per_mol_n2 = 0.0280134000;
constexpr double molar_mass_kg_per_mol_ar = 0.0399480000;
constexpr double molar_mass_kg_per_mol_co2 = 0.0440095000;
constexpr double molar_mass_kg_per_mol_h2o = 0.0180152800;
constexpr double ambient_static_temperature_k = 300.0;
constexpr double ambient_static_pressure_pa = 101325.0;

struct gas_s
{
    double mol_ratio_c8h18;
    double mol_ratio_o2;
    double mol_ratio_n2;
    double mol_ratio_ar;
    double mol_ratio_co2;
    double mol_ratio_h2o;
    double static_temperature_k;
    double mass_kg;
    double momentum_kg_m_per_s;
};

constexpr struct gas_s ambient_gas_air = {
    .mol_ratio_n2 = 0.78,
    .mol_ratio_o2 = 0.21,
    .mol_ratio_ar = 0.01,
    .static_temperature_k = ambient_static_temperature_k
};

static double
calc_mixed_molar_mass_kg_per_mol(const struct gas_s* self)
{
    return self->mol_ratio_c8h18 * molar_mass_kg_per_mol_c8h18
         + self->mol_ratio_o2 * molar_mass_kg_per_mol_o2
         + self->mol_ratio_n2 * molar_mass_kg_per_mol_n2
         + self->mol_ratio_ar * molar_mass_kg_per_mol_ar
         + self->mol_ratio_co2 * molar_mass_kg_per_mol_co2
         + self->mol_ratio_h2o * molar_mass_kg_per_mol_h2o;
}

static double
calc_mixed_cp_j_per_mol_k(const struct gas_s* self)
{
    return self->mol_ratio_c8h18 * calc_cp_c8h18_j_per_mol_k(self->static_temperature_k)
         + self->mol_ratio_o2 * calc_cp_o2_j_per_mol_k(self->static_temperature_k)
         + self->mol_ratio_n2 * calc_cp_n2_j_per_mol_k(self->static_temperature_k)
         + self->mol_ratio_ar * calc_cp_ar_j_per_mol_k(self->static_temperature_k)
         + self->mol_ratio_co2 * calc_cp_co2_j_per_mol_k(self->static_temperature_k)
         + self->mol_ratio_h2o * calc_cp_h2o_j_per_mol_k(self->static_temperature_k);
}

static double
calc_mixed_cv_j_per_mol_k(const struct gas_s* self)
{
    return calc_cv_j_per_mol_k(calc_mixed_cp_j_per_mol_k(self));
}

static double
calc_mixed_gamma(const struct gas_s* self)
{
    return calc_gamma(calc_mixed_cp_j_per_mol_k(self));
}

/*
 *      m
 * n = ---
 *      M
 */

static double
calc_moles(const struct gas_s* self)
{
    double m = self->mass_kg;
    double M = calc_mixed_molar_mass_kg_per_mol(self);
    return m / M;
}

static double
calc_total_cp_j_per_k(const struct gas_s* self)
{
    return calc_moles(self) * calc_mixed_cp_j_per_mol_k(self);
}

static double
calc_total_cv_j_per_k(const struct gas_s* self)
{
    return calc_moles(self) * calc_mixed_cv_j_per_mol_k(self);
}

/*
 *       R
 * Rs = ---
 *       M
 */

static double
calc_specific_gas_constant_j_per_kg_k(const struct gas_s* self)
{
    double R = universal_gas_constant_j_per_mol_k;
    double M = calc_mixed_molar_mass_kg_per_mol(self);
    return R / M;
}

/*
 *      p
 * u = ---
 *      m
 */

static double
calc_bulk_flow_velocity_m_per_s(const struct gas_s* self)
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
calc_bulk_speed_of_sound_m_per_s(const struct gas_s* self)
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
calc_bulk_mach(const struct gas_s* self)
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
calc_max_bulk_momentum_kg_m_per_s(const struct gas_s* self)
{
    double m = self->mass_kg;
    double a = calc_bulk_speed_of_sound_m_per_s(self);
    return m * a;
}

/*       w1 * x1 + w2 * x2
 * mix = -----------------
 *            w1 + w
 */

static double
calc_mix(double value1, double weight1, double value2, double weight2)
{
    return (value1 * weight1 + value2 * weight2) / (weight1 + weight2);
}

static void
mix_in_gas(struct gas_s* self, const struct gas_s* mail)
{
    self->mass_kg += mail->mass_kg;
    self->momentum_kg_m_per_s += mail->momentum_kg_m_per_s;
    self->mol_ratio_c8h18 = calc_mix(self->mol_ratio_c8h18, calc_moles(self), mail->mol_ratio_c8h18, calc_moles(mail));
    self->mol_ratio_o2 = calc_mix(self->mol_ratio_o2, calc_moles(self), mail->mol_ratio_o2, calc_moles(mail));
    self->mol_ratio_n2 = calc_mix(self->mol_ratio_n2, calc_moles(self), mail->mol_ratio_n2, calc_moles(mail));
    self->mol_ratio_ar = calc_mix(self->mol_ratio_ar, calc_moles(self), mail->mol_ratio_ar, calc_moles(mail));
    self->mol_ratio_co2 = calc_mix(self->mol_ratio_co2, calc_moles(self), mail->mol_ratio_co2, calc_moles(mail));
    self->mol_ratio_h2o = calc_mix(self->mol_ratio_h2o, calc_moles(self), mail->mol_ratio_h2o, calc_moles(mail));
    self->static_temperature_k = calc_mix(self->static_temperature_k, calc_total_cv_j_per_k(self), mail->static_temperature_k, calc_total_cv_j_per_k(mail));
}

static void
clamp_momentum(struct gas_s* self)
{
    double max_bulk_momentum_kg_m_per_s = calc_max_bulk_momentum_kg_m_per_s(self);
    self->momentum_kg_m_per_s = clamp(self->momentum_kg_m_per_s, -max_bulk_momentum_kg_m_per_s, max_bulk_momentum_kg_m_per_s);
}
