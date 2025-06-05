static constexpr double gas_molar_mass_kg_per_mol_c8h18 = 0.1142285200;
static constexpr double gas_molar_mass_kg_per_mol_o2 = 0.0319988000;
static constexpr double gas_molar_mass_kg_per_mol_n2 = 0.0280134000;
static constexpr double gas_molar_mass_kg_per_mol_ar = 0.0399480000;
static constexpr double gas_molar_mass_kg_per_mol_co2 = 0.0440095000;
static constexpr double gas_molar_mass_kg_per_mol_h2o = 0.0180152800;
static constexpr double gas_ambient_static_temperature_k = 300.0;
static constexpr double gas_ambient_static_pressure_pa = 101325.0;

struct gas_s
{
#define X(M) double mol_ratio_##M;
    GAMMA_MOLECULES
#undef X
    double static_temperature_k;
    double mass_kg;
    double momentum_kg_m_per_s;
};


static constexpr struct gas_s gas_ambient_air = {
    .mol_ratio_n2 = 0.78,
    .mol_ratio_o2 = 0.21,
    .mol_ratio_ar = 0.01,
    .static_temperature_k = gas_ambient_static_temperature_k
};

static double
calc_mixed_molar_mass_kg_per_mol(const struct gas_s* self)
{
    double result = 0.0;
#define X(M) result += self->mol_ratio_##M * gas_molar_mass_kg_per_mol_##M;
    GAMMA_MOLECULES
#undef X
    return result;
}

static double
calc_mixed_cp_j_per_mol_k(const struct gas_s* self)
{
    double result = 0.0;
#define X(M) result += self->mol_ratio_##M * lookup_cp_##M##_j_per_mol_k(self->static_temperature_k);
    GAMMA_MOLECULES
#undef X
    return result;
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
    double R = gamma_universal_gas_constant_j_per_mol_k;
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
    double self_moles = calc_moles(self);
    double mail_moles = calc_moles(mail);
    double self_total_cv_j_per_k = calc_total_cv_j_per_k(self);
    double mail_total_cv_j_per_k = calc_total_cv_j_per_k(mail);
#define X(M) self->mol_ratio_##M = calc_mix(self->mol_ratio_##M, self_moles, mail->mol_ratio_##M, mail_moles);
    GAMMA_MOLECULES
#undef X
    self->static_temperature_k = calc_mix(self->static_temperature_k, self_total_cv_j_per_k, mail->static_temperature_k, mail_total_cv_j_per_k);
}

static void
clamp_momentum(struct gas_s* self)
{
    double max_bulk_momentum_kg_m_per_s = calc_max_bulk_momentum_kg_m_per_s(self);
    self->momentum_kg_m_per_s = clamp(self->momentum_kg_m_per_s, -max_bulk_momentum_kg_m_per_s, max_bulk_momentum_kg_m_per_s);
}
