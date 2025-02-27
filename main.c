#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static double
clamp(double value, double x, double y)
{
    return value < x ? x : value > y ? y : value;
}

constexpr size_t max_edges_per_node = 16;
constexpr size_t nodes_per_engine = 16;
constexpr size_t cache_line_bytes = 128;
constexpr double audio_sample_rate_hz = 44100.0;
constexpr double dt_s = 1.0 / audio_sample_rate_hz;

/*
 *  2 c8h18 + 25 o2 -> 16 co2 + 18 h2o
 */

constexpr double universal_gas_constant_j_per_mol_k = 8.3144598;
constexpr double molar_mass_kg_per_mol_c8h18 = 0.1142285200;
constexpr double molar_mass_kg_per_mol_o2 = 0.0319988000;
constexpr double molar_mass_kg_per_mol_n2 = 0.0280134000;
constexpr double molar_mass_kg_per_mol_ar = 0.0399480000;
constexpr double molar_mass_kg_per_mol_co2 = 0.0440095000;
constexpr double molar_mass_kg_per_mol_h2o = 0.0180152800;

struct gas_t
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

struct chamber_t
{
    struct gas_t gas;
    double volume_m3;
    double nozzle_max_flow_area_m2;
    double nozzle_open_ratio;
};

struct piston_t
{
    struct chamber_t chamber;
};

enum tag_t
{
    is_chamber, is_piston,
};

struct node_t
{
    alignas(cache_line_bytes) struct
    {
        enum tag_t tag;
        union
        {
            struct chamber_t chamber;
            struct piston_t piston;
        }
        as;
        struct node_t* next[max_edges_per_node];
    };
};

struct engine_t
{
    struct node_t nodes[nodes_per_engine];
};

/* iso-octane 2,2,4-trimethylpentane (of gasoline), [1, p. 103] */
constexpr double cp_weights_lower_c8h18[] = {
    -1.688758565e+05, +3.126903227e+03, -2.123502828e+01, +1.489151508e-01, -1.151180135e-04, +4.473216170e-08, -5.554882070e-12
};
constexpr double cp_weights_upper_c8h18[] = {
    +1.352765032e+07, -4.663370340e+04, +7.795313180e+01, +1.423729984e-02, -5.073593910e-06, +7.248232970e-10, -3.819190110e-14
};

/* oxygen (of air) [1, p. 166] */
constexpr double cp_weights_lower_o2[] = {
    -3.425563420e+04, +4.847000970e+02, +1.119010961e+00, +4.293889240e-03, -6.836300520e-07, -2.023372700e-09, +1.039040018e-12
};
constexpr double cp_weights_upper_o2[] = {
    -1.037939022e+06, +2.344830282e+03, +1.819732036e+00, +1.267847582e-03, -2.188067988e-07, +2.053719572e-11, -8.193467050e-16
};

/* nitrogen (of air) [1, p. 156] */
constexpr double cp_weights_lower_n2[] = {
    +2.210371497e+04, -3.818461820e+02, +6.082738360e+00, -8.530914410e-03, +1.384646189e-05, -9.625793620e-09, +2.519705809e-12
};
constexpr double cp_weights_upper_n2[] = {
    +5.877124060e+05, -2.239249073e+03, +6.066949220e+00, -6.139685500e-04, +1.491806679e-07, -1.923105485e-11, +1.061954386e-15
};

/* argon (of air) [1, p. 55] */
constexpr double cp_weights_lower_ar[] = {
    +0.000000000e+00, +0.000000000e+00, +2.500000000e+00, +0.000000000e+00, +0.000000000e+00, +0.000000000e+00, +0.000000000e+00
};
constexpr double cp_weights_upper_ar[] = {
    +2.010538475e+01, -5.992661070e-02, +2.500069401e+00, -3.992141160e-08, +1.205272140e-11, -1.819015576e-15, +1.078576636e-19
};

/* carbon-dioxide (of combustion) [1, p. 85] */
constexpr double cp_weights_lower_co2[] = {
    +4.943650540e+04, -6.264116010e+02, +5.301725240e+00, +2.503813816e-03, -2.127308728e-07, -7.689988780e-10, +2.849677801e-13
};
constexpr double cp_weights_upper_co2[] = {
    +1.176962419e+05, -1.788791477e+03, +8.291523190e+00, -9.223156780e-05, +4.863676880e-09, -1.891053312e-12, +6.330036590e-16
};

/* water (of combustion) [1, p. 131] */
constexpr double cp_weights_lower_h2o[] = {
    -3.947960830e+04, +5.755731020e+02, +9.317826530e-01, +7.222712860e-03, -7.342557370e-06, +4.955043490e-09, -1.336933246e-12
};
constexpr double cp_weights_upper_h2o[] = {
    +1.034972096e+06, -2.412698562e+03, +4.646110780e+00, +2.291998307e-03, -6.836830480e-07, +9.426468930e-11, -4.822380530e-15
};

/* eq. 1, [1, p. 43]
 *
 *                 -2         -1         0          1          2          3          4
 * cp = R * [a0 * T   + a1 * T   + a2 * T   + a3 * T   + a4 * T   + a5 * T   + a6 * T ]
 *
 */

static double
calc_cp_j_per_mol_k(double static_temperature_k, const double* lower, const double* upper)
{
    static_temperature_k = clamp(static_temperature_k, 200.0, 6000.0);
    const double* a = static_temperature_k < 1000.0 ? lower : upper;
    return (a[0] * pow(static_temperature_k, -2.0)
          + a[1] * pow(static_temperature_k, -1.0)
          + a[2] * pow(static_temperature_k, +0.0)
          + a[3] * pow(static_temperature_k, +1.0)
          + a[4] * pow(static_temperature_k, +2.0)
          + a[5] * pow(static_temperature_k, +3.0)
          + a[6] * pow(static_temperature_k, +4.0)) * universal_gas_constant_j_per_mol_k;
}

static double
calc_cp_n2_j_per_mol_k(double static_temperature_k)
{
    return calc_cp_j_per_mol_k(static_temperature_k, cp_weights_lower_n2, cp_weights_upper_n2);
}

static double
calc_cp_o2_j_per_mol_k(double static_temperature_k)
{
    return calc_cp_j_per_mol_k(static_temperature_k, cp_weights_lower_o2, cp_weights_upper_o2);
}

static double
calc_cp_ar_j_per_mol_k(double static_temperature_k)
{
    return calc_cp_j_per_mol_k(static_temperature_k, cp_weights_lower_ar, cp_weights_upper_ar);
}

static double
calc_cp_c8h18_j_per_mol_k(double static_temperature_k)
{
    return calc_cp_j_per_mol_k(static_temperature_k, cp_weights_lower_c8h18, cp_weights_upper_c8h18);
}

static double
calc_cp_co2_j_per_mol_k(double static_temperature_k)
{
    return calc_cp_j_per_mol_k(static_temperature_k, cp_weights_lower_co2, cp_weights_upper_co2);
}

static double
calc_cp_h2o_j_per_mol_k(double static_temperature_k)
{
    return calc_cp_j_per_mol_k(static_temperature_k, cp_weights_lower_h2o, cp_weights_upper_h2o);
}

/*
 *
 * cv = cp - R
 *
 */

static double
calc_cv_j_per_mol_k(double cp_j_per_mol_k)
{
    return cp_j_per_mol_k - universal_gas_constant_j_per_mol_k;
}

/*
 *
 * y = cp / cv
 *
 */

static double
calc_gamma(double cp_j_per_mol_k)
{
    return cp_j_per_mol_k / calc_cv_j_per_mol_k(cp_j_per_mol_k);
}

static double
calc_gamma_n2(double static_temperature_k)
{
    return calc_gamma(calc_cp_n2_j_per_mol_k(static_temperature_k));
}

static double
calc_gamma_o2(double static_temperature_k)
{
    return calc_gamma(calc_cp_o2_j_per_mol_k(static_temperature_k));
}

static double
calc_gamma_ar(double static_temperature_k)
{
    return calc_gamma(calc_cp_ar_j_per_mol_k(static_temperature_k));
}

static double
calc_gamma_c8h18(double static_temperature_k)
{
    return calc_gamma(calc_cp_c8h18_j_per_mol_k(static_temperature_k));
}

static double
calc_gamma_co2(double static_temperature_k)
{
    return calc_gamma(calc_cp_co2_j_per_mol_k(static_temperature_k));
}

static double
calc_gamma_h2o(double static_temperature_k)
{
    return calc_gamma(calc_cp_h2o_j_per_mol_k(static_temperature_k));
}

static void
plot_gamma(void)
{
    FILE* file = fopen("plot/gamma.txt", "w");
    for(double static_temperature_k = 0; static_temperature_k < 6'000; static_temperature_k += 100.0)
    {
        fprintf(
            file,
            "%f " /* 1: static temperature */
            "%f " /* 2: n2 */
            "%f " /* 3: o2 */
            "%f " /* 4: ar */
            "%f " /* 5: c8h18 */
            "%f " /* 6: co2 */
            "%f " /* 7: h2o */
            "\n"
            , static_temperature_k
            , calc_gamma_n2(static_temperature_k)
            , calc_gamma_o2(static_temperature_k)
            , calc_gamma_ar(static_temperature_k)
            , calc_gamma_c8h18(static_temperature_k)
            , calc_gamma_co2(static_temperature_k)
            , calc_gamma_h2o(static_temperature_k)
        );
    }
    fclose(file);
}

static double
calc_mixed_molar_mass_kg_per_mol(const struct gas_t* self)
{
    return self->mol_ratio_c8h18 * molar_mass_kg_per_mol_c8h18
         + self->mol_ratio_o2 * molar_mass_kg_per_mol_o2
         + self->mol_ratio_n2 * molar_mass_kg_per_mol_n2
         + self->mol_ratio_ar * molar_mass_kg_per_mol_ar
         + self->mol_ratio_co2 * molar_mass_kg_per_mol_co2
         + self->mol_ratio_h2o * molar_mass_kg_per_mol_h2o;
}

static double
calc_mixed_cp_j_per_mol_k(const struct gas_t* self)
{
    return self->mol_ratio_c8h18 * calc_cp_c8h18_j_per_mol_k(self->static_temperature_k)
         + self->mol_ratio_o2 * calc_cp_o2_j_per_mol_k(self->static_temperature_k)
         + self->mol_ratio_n2 * calc_cp_n2_j_per_mol_k(self->static_temperature_k)
         + self->mol_ratio_ar * calc_cp_ar_j_per_mol_k(self->static_temperature_k)
         + self->mol_ratio_co2 * calc_cp_co2_j_per_mol_k(self->static_temperature_k)
         + self->mol_ratio_h2o * calc_cp_h2o_j_per_mol_k(self->static_temperature_k);
}

static double
calc_mixed_cv_j_per_mol_k(const struct gas_t* self)
{
    return calc_cv_j_per_mol_k(calc_mixed_cp_j_per_mol_k(self));
}

static double
calc_mixed_gamma(const struct gas_t* self)
{
    return calc_gamma(calc_mixed_cp_j_per_mol_k(self));
}

/*
 *      m
 * n = ---
 *      M
 */

static double
calc_moles(const struct gas_t* self)
{
    double m = self->mass_kg;
    double M = calc_mixed_molar_mass_kg_per_mol(self);
    return m / M;
}

static double
calc_total_cp_j_per_k(const struct gas_t* self)
{
    return calc_moles(self) * calc_mixed_cp_j_per_mol_k(self);
}

static double
calc_total_cv_j_per_k(const struct gas_t* self)
{
    return calc_moles(self) * calc_mixed_cv_j_per_mol_k(self);
}

/*
 *       R
 * Rs = ---
 *       M
 */

static double
calc_specific_gas_constant_j_per_kg_k(const struct gas_t* self)
{
    double R = universal_gas_constant_j_per_mol_k;
    double M = calc_mixed_molar_mass_kg_per_mol(self);
    return R / M;
}

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

/*
 *      p
 * u = ---
 *      m
 */

static double
calc_bulk_flow_velocity_m_per_s(const struct gas_t* self)
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
calc_bulk_speed_of_sound_m_per_s(const struct gas_t* self)
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
calc_bulk_mach(const struct gas_t* self)
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
calc_max_bulk_momentum_kg_m_per_s(const struct gas_t* self)
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
clamp_momentum(struct gas_t* self)
{
    double max_bulk_momentum_kg_m_per_s = calc_max_bulk_momentum_kg_m_per_s(self);
    self->momentum_kg_m_per_s = clamp(self->momentum_kg_m_per_s, -max_bulk_momentum_kg_m_per_s, max_bulk_momentum_kg_m_per_s);
}

static void
mix_in_gas(struct gas_t* self, const struct gas_t* mail)
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
remove_gas(struct gas_t* self, const struct gas_t* mail)
{
    double y = calc_mixed_gamma(self);
    self->static_temperature_k *= pow((self->mass_kg - mail->mass_kg) / self->mass_kg, y - 1.0);
    self->mass_kg -= mail->mass_kg;
    self->momentum_kg_m_per_s -= mail->momentum_kg_m_per_s;
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
    if(Pt0 - Pt1 < 1.0)
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

static double
calc_nozzle_flow_area_m2(const struct chamber_t* self)
{
    return self->nozzle_max_flow_area_m2 * self->nozzle_open_ratio;
}

static void
flow(struct chamber_t* x, struct chamber_t* y)
{
    double nozzle_flow_area_m2 = calc_nozzle_flow_area_m2(x);
    if(nozzle_flow_area_m2 > 0.0)
    {
        if(calc_total_pressure_pa(x) > calc_total_pressure_pa(y))
        {
            /* always flows from chamber x to chamber y */
        }
        else
        {
            struct chamber_t* temp = x;
            x = y;
            y = temp;
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
        remove_gas(&x->gas, &mail);
        mix_in_gas(&y->gas, &mail);
        clamp_momentum(&x->gas);
        clamp_momentum(&y->gas);
    }
}

static struct engine_t flow_test = {
    .nodes = {
        { .tag = is_chamber, .as.chamber = { .gas = { .mol_ratio_n2 = 0.78, .mol_ratio_o2 = 0.21, .mol_ratio_ar = 0.01, .static_temperature_k = 599.0, .mass_kg = 1.0 }, .volume_m3 = 1.0, .nozzle_max_flow_area_m2 = 0.02, .nozzle_open_ratio = 1.0 } },
        { .tag = is_chamber, .as.chamber = { .gas = { .mol_ratio_n2 = 0.78, .mol_ratio_o2 = 0.21, .mol_ratio_ar = 0.01, .static_temperature_k = 300.0, .mass_kg = 1.0 }, .volume_m3 = 5.0, .nozzle_max_flow_area_m2 = 0.00, .nozzle_open_ratio = 0.0 } },
    }
};

static void
plot_simple_flow(void)
{
    FILE* file = fopen("plot/simple_flow.txt", "w");
    for(size_t cycle = 0; cycle < 75'000; cycle++)
    {
        struct chamber_t* x = &flow_test.nodes[0].as.chamber;
        struct chamber_t* y = &flow_test.nodes[1].as.chamber;
        struct chamber_t* upper = calc_total_pressure_pa(x) > calc_total_pressure_pa(y) ? x : y;
        struct chamber_t* lower = calc_total_pressure_pa(x) > calc_total_pressure_pa(y) ? y : x;
        double direction = upper == x ? 1.0 : -1.0;
        double nozzle_flow_area_m2 = calc_nozzle_flow_area_m2(x);
        flow(x, y);
        fprintf(
            file,
            "%f " /*  1: time */
            "%f " /*  2: total pressure chamber x */
            "%f " /*  3: total pressure chamber y */
            "%f " /*  4: total temperature chamber x */
            "%f " /*  5: total temperature chamber y */
            "%f " /*  6: static temperature chamber x */
            "%f " /*  7: static temperature chamber y */
            "%f " /*  8: mass chamber x */
            "%f " /*  9: mass chamber y */
            "%f " /* 10: momentum chamber x */
            "%f " /* 11: momentum chamber y */
            "%f " /* 12: bulk mach chamber x */
            "%f " /* 13: bulk mach chamber y */
            "%f " /* 14: nozzle mach */
            "%f " /* 15: nozzle flow velocity */
            "%f " /* 16: nozzle mass flow rate */
            "\n"
            /*  1: */ , cycle * dt_s
            /*  2: */ , calc_total_pressure_pa(x)
            /*  3: */ , calc_total_pressure_pa(y)
            /*  4: */ , calc_total_temperature_k(x)
            /*  5: */ , calc_total_temperature_k(y)
            /*  6: */ , x->gas.static_temperature_k
            /*  7: */ , y->gas.static_temperature_k
            /*  8: */ , x->gas.mass_kg
            /*  9: */ , y->gas.mass_kg
            /* 10: */ , x->gas.momentum_kg_m_per_s
            /* 11: */ , y->gas.momentum_kg_m_per_s
            /* 12: */ , calc_bulk_mach(&x->gas)
            /* 13: */ , calc_bulk_mach(&y->gas)
            /* 14: */ , calc_nozzle_mach(upper, lower) * direction
            /* 15: */ , calc_nozzle_flow_velocity_m_per_s(upper, lower) * direction
            /* 16: */ , calc_nozzle_mass_flow_rate_kg_per_s(upper, lower, nozzle_flow_area_m2) * direction
        );
    }
    fclose(file);
}

int
main(void)
{
    plot_gamma();
    plot_simple_flow();
}

/* [1] B. Mcbride, M. Zehe, and S. Gordon, “NASA Glenn Coefficients for Calculating Thermodynamic Properties of Individual Species”, 2002.
 *     Available: https://ntrs.nasa.gov/api/citations/20020085330/downloads/20020085330.pdf
 *
 */
