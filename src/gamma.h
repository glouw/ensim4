constexpr double g_gamma_universal_gas_constant_j_per_mol_k = 8.3144598;
constexpr size_t g_gamma_cp_precompute_buffer_size = 8192;

static double g_cp_n2_j_per_mol_k[g_gamma_cp_precompute_buffer_size] = {};
static double g_cp_o2_j_per_mol_k[g_gamma_cp_precompute_buffer_size] = {};
static double g_cp_ar_j_per_mol_k[g_gamma_cp_precompute_buffer_size] = {};
static double g_cp_c8h18_j_per_mol_k[g_gamma_cp_precompute_buffer_size] = {};
static double g_cp_co2_j_per_mol_k[g_gamma_cp_precompute_buffer_size] = {};
static double g_cp_h2o_j_per_mol_k[g_gamma_cp_precompute_buffer_size] = {};

/* iso-octane 2,2,4-trimethylpentane (of gasoline), [1, p. 103] */
constexpr double g_gamma_cp_weights_lower_c8h18[] = {
    -1.688758565e+05, +3.126903227e+03, -2.123502828e+01, +1.489151508e-01, -1.151180135e-04, +4.473216170e-08, -5.554882070e-12
};
constexpr double g_gamma_cp_weights_upper_c8h18[] = {
    +1.352765032e+07, -4.663370340e+04, +7.795313180e+01, +1.423729984e-02, -5.073593910e-06, +7.248232970e-10, -3.819190110e-14
};

/* oxygen (of air) [1, p. 166] */
constexpr double g_gamma_cp_weights_lower_o2[] = {
    -3.425563420e+04, +4.847000970e+02, +1.119010961e+00, +4.293889240e-03, -6.836300520e-07, -2.023372700e-09, +1.039040018e-12
};
constexpr double g_gamma_cp_weights_upper_o2[] = {
    -1.037939022e+06, +2.344830282e+03, +1.819732036e+00, +1.267847582e-03, -2.188067988e-07, +2.053719572e-11, -8.193467050e-16
};

/* nitrogen (of air) [1, p. 156] */
constexpr double g_gamma_cp_weights_lower_n2[] = {
    +2.210371497e+04, -3.818461820e+02, +6.082738360e+00, -8.530914410e-03, +1.384646189e-05, -9.625793620e-09, +2.519705809e-12
};
constexpr double g_gamma_cp_weights_upper_n2[] = {
    +5.877124060e+05, -2.239249073e+03, +6.066949220e+00, -6.139685500e-04, +1.491806679e-07, -1.923105485e-11, +1.061954386e-15
};

/* argon (of air) [1, p. 55] */
constexpr double g_gamma_cp_weights_lower_ar[] = {
    +0.000000000e+00, +0.000000000e+00, +2.500000000e+00, +0.000000000e+00, +0.000000000e+00, +0.000000000e+00, +0.000000000e+00
};
constexpr double g_gamma_cp_weights_upper_ar[] = {
    +2.010538475e+01, -5.992661070e-02, +2.500069401e+00, -3.992141160e-08, +1.205272140e-11, -1.819015576e-15, +1.078576636e-19
};

/* carbon-dioxide (of combustion) [1, p. 85] */
constexpr double g_gamma_cp_weights_lower_co2[] = {
    +4.943650540e+04, -6.264116010e+02, +5.301725240e+00, +2.503813816e-03, -2.127308728e-07, -7.689988780e-10, +2.849677801e-13
};
constexpr double g_gamma_cp_weights_upper_co2[] = {
    +1.176962419e+05, -1.788791477e+03, +8.291523190e+00, -9.223156780e-05, +4.863676880e-09, -1.891053312e-12, +6.330036590e-16
};

/* water (of combustion) [1, p. 131] */
constexpr double g_gamma_cp_weights_lower_h2o[] = {
    -3.947960830e+04, +5.755731020e+02, +9.317826530e-01, +7.222712860e-03, -7.342557370e-06, +4.955043490e-09, -1.336933246e-12
};
constexpr double g_gamma_cp_weights_upper_h2o[] = {
    +1.034972096e+06, -2.412698562e+03, +4.646110780e+00, +2.291998307e-03, -6.836830480e-07, +9.426468930e-11, -4.822380530e-15
};

/* eq. 1, [1, p. 43]
 *
 *                 -2       -1        0        1        2        3        4
 * cp = R * [ a0 * T + a1 * T + a2 * T + a3 * T + a4 * T + a5 * T + a6 * T ]
 *
 */

static double
calc_cp_j_per_mol_k(double static_temperature_k, const double* lower, const double* upper)
{
    double T1 = clamp(static_temperature_k, 200.0, 6000.0);
    const double* a = T1 < 1000.0 ? lower : upper;
    double T2 = T1 * T1;
    double T3 = T2 * T1;
    double T4 = T3 * T1;
    double inv_T1 = 1.0 / T1;
    double inv_T2 = inv_T1 * inv_T1;
    return (a[0] * inv_T2 + a[1] * inv_T1 + a[2] + a[3] * T1 + a[4] * T2 + a[5] * T3 + a[6] * T4) * g_gamma_universal_gas_constant_j_per_mol_k;
}

static double
calc_cp_n2_j_per_mol_k(double static_temperature_k)
{
    return calc_cp_j_per_mol_k(static_temperature_k, g_gamma_cp_weights_lower_n2, g_gamma_cp_weights_upper_n2);
}

static double
calc_cp_o2_j_per_mol_k(double static_temperature_k)
{
    return calc_cp_j_per_mol_k(static_temperature_k, g_gamma_cp_weights_lower_o2, g_gamma_cp_weights_upper_o2);
}

static double
calc_cp_ar_j_per_mol_k(double static_temperature_k)
{
    return calc_cp_j_per_mol_k(static_temperature_k, g_gamma_cp_weights_lower_ar, g_gamma_cp_weights_upper_ar);
}

static double
calc_cp_c8h18_j_per_mol_k(double static_temperature_k)
{
    return calc_cp_j_per_mol_k(static_temperature_k, g_gamma_cp_weights_lower_c8h18, g_gamma_cp_weights_upper_c8h18);
}

static double
calc_cp_co2_j_per_mol_k(double static_temperature_k)
{
    return calc_cp_j_per_mol_k(static_temperature_k, g_gamma_cp_weights_lower_co2, g_gamma_cp_weights_upper_co2);
}

static double
calc_cp_h2o_j_per_mol_k(double static_temperature_k)
{
    return calc_cp_j_per_mol_k(static_temperature_k, g_gamma_cp_weights_lower_h2o, g_gamma_cp_weights_upper_h2o);
}

static void
precompute_cp()
{
    for(size_t i = 0; i < g_gamma_cp_precompute_buffer_size; i++)
    {
        g_cp_n2_j_per_mol_k[i] = calc_cp_n2_j_per_mol_k(i);
        g_cp_o2_j_per_mol_k[i] = calc_cp_o2_j_per_mol_k(i);
        g_cp_ar_j_per_mol_k[i] = calc_cp_ar_j_per_mol_k(i);
        g_cp_c8h18_j_per_mol_k[i] = calc_cp_c8h18_j_per_mol_k(i);
        g_cp_co2_j_per_mol_k[i] = calc_cp_co2_j_per_mol_k(i);
        g_cp_h2o_j_per_mol_k[i] = calc_cp_h2o_j_per_mol_k(i);
    }
}

/*
 *
 * cv = cp - R
 *
 */

static double
calc_cv_j_per_mol_k(double cp_j_per_mol_k)
{
    return cp_j_per_mol_k - g_gamma_universal_gas_constant_j_per_mol_k;
}

static double
lookup_cp_n2_j_per_mol_k(size_t static_temperature_k)
{
    return g_cp_n2_j_per_mol_k[static_temperature_k];
}

static double
lookup_cp_o2_j_per_mol_k(size_t static_temperature_k)
{
    return g_cp_o2_j_per_mol_k[static_temperature_k];
}

static double
lookup_cp_ar_j_per_mol_k(size_t static_temperature_k)
{
    return g_cp_ar_j_per_mol_k[static_temperature_k];
}

static double
lookup_cp_c8h18_j_per_mol_k(size_t static_temperature_k)
{
    return g_cp_c8h18_j_per_mol_k[static_temperature_k];
}

static double
lookup_cp_co2_j_per_mol_k(size_t static_temperature_k)
{
    return g_cp_co2_j_per_mol_k[static_temperature_k];
}

static double
lookup_cp_h2o_j_per_mol_k(size_t static_temperature_k)
{
    return g_cp_h2o_j_per_mol_k[static_temperature_k];
}

/*
 *
 * y = cp / cv
 *
 */

static double
lookup_gamma(double cp_j_per_mol_k)
{
    return cp_j_per_mol_k / calc_cv_j_per_mol_k(cp_j_per_mol_k);
}

static double
lookup_gamma_n2(double static_temperature_k)
{
    return lookup_gamma(lookup_cp_n2_j_per_mol_k(static_temperature_k));
}

static double
lookup_gamma_o2(double static_temperature_k)
{
    return lookup_gamma(lookup_cp_o2_j_per_mol_k(static_temperature_k));
}

static double
lookup_gamma_ar(double static_temperature_k)
{
    return lookup_gamma(lookup_cp_ar_j_per_mol_k(static_temperature_k));
}

static double
lookup_gamma_c8h18(double static_temperature_k)
{
    return lookup_gamma(lookup_cp_c8h18_j_per_mol_k(static_temperature_k));
}

static double
lookup_gamma_co2(double static_temperature_k)
{
    return lookup_gamma(lookup_cp_co2_j_per_mol_k(static_temperature_k));
}

static double
lookup_gamma_h2o(double static_temperature_k)
{
    return lookup_gamma(lookup_cp_h2o_j_per_mol_k(static_temperature_k));
}

/* [1] B. Mcbride, M. Zehe, and S. Gordon, “NASA Glenn Coefficients for Calculating Thermodynamic Properties of Individual Species”, 2002.
 *     Available: https://ntrs.nasa.gov/api/citations/20020085330/downloads/20020085330.pdf
 */
