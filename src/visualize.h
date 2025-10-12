[[maybe_unused]]
static void
visualize_gamma()
{
    FILE* file = fopen("visualize/gamma.txt", "w");
    for(double static_temperature_k = 0; static_temperature_k < 6'000; static_temperature_k += 100.0)
    {
        double gamma_n2 = lookup_gamma_n2(static_temperature_k);
        double gamma_o2 = lookup_gamma_o2(static_temperature_k);
        double gamma_ar = lookup_gamma_ar(static_temperature_k);
        double gamma_c8h18 = lookup_gamma_c8h18(static_temperature_k);
        double gamma_co2 = lookup_gamma_co2(static_temperature_k);
        double gamma_h2o = lookup_gamma_h2o(static_temperature_k);
        double cp_n2_j_per_mol_k = lookup_cp_n2_j_per_mol_k(static_temperature_k);
        double cp_o2_j_per_mol_k = lookup_cp_o2_j_per_mol_k(static_temperature_k);
        double cp_ar_j_per_mol_k = lookup_cp_ar_j_per_mol_k(static_temperature_k);
        double cp_c8h18_j_per_mol_k = lookup_cp_c8h18_j_per_mol_k(static_temperature_k);
        double cp_co2_j_per_mol_k = lookup_cp_co2_j_per_mol_k(static_temperature_k);
        double cp_h2o_j_per_mol_k = lookup_cp_h2o_j_per_mol_k(static_temperature_k);
        double cv_n2_j_per_mol_k = calc_cv_j_per_mol_k(cp_n2_j_per_mol_k);
        double cv_o2_j_per_mol_k = calc_cv_j_per_mol_k(cp_o2_j_per_mol_k);
        double cv_ar_j_per_mol_k = calc_cv_j_per_mol_k(cp_ar_j_per_mol_k);
        double cv_c8h18_j_per_mol_k = calc_cv_j_per_mol_k(cp_c8h18_j_per_mol_k);
        double cv_co2_j_per_mol_k = calc_cv_j_per_mol_k(cp_co2_j_per_mol_k);
        double cv_h2o_j_per_mol_k = calc_cv_j_per_mol_k(cp_h2o_j_per_mol_k);
        fprintf(
            file,
            "%f " /*  1 */
            "%f " /*  2 */
            "%f " /*  3 */
            "%f " /*  4 */
            "%f " /*  5 */
            "%f " /*  6 */
            "%f " /*  7 */
            "%f " /*  8 */
            "%f " /*  9 */
            "%f " /* 10 */
            "%f " /* 11 */
            "%f " /* 12 */
            "%f " /* 13 */
            "%f " /* 14 */
            "%f " /* 15 */
            "%f " /* 16 */
            "%f " /* 17 */
            "%f " /* 18 */
            "%f " /* 19 */
            "\n"
            , /*  1 */ static_temperature_k
            , /*  2 */ gamma_n2
            , /*  3 */ gamma_o2
            , /*  4 */ gamma_ar
            , /*  5 */ gamma_c8h18
            , /*  6 */ gamma_co2
            , /*  7 */ gamma_h2o
            , /*  8 */ cp_n2_j_per_mol_k
            , /*  9 */ cp_o2_j_per_mol_k
            , /* 10 */ cp_ar_j_per_mol_k
            , /* 11 */ cp_c8h18_j_per_mol_k
            , /* 12 */ cp_co2_j_per_mol_k
            , /* 13 */ cp_h2o_j_per_mol_k
            , /* 14 */ cv_n2_j_per_mol_k
            , /* 15 */ cv_o2_j_per_mol_k
            , /* 16 */ cv_ar_j_per_mol_k
            , /* 17 */ cv_c8h18_j_per_mol_k
            , /* 18 */ cv_co2_j_per_mol_k
            , /* 19 */ cv_h2o_j_per_mol_k
        );
    }
    fclose(file);
}

[[maybe_unused]]
static void
visualize_chamber_s()
{
    struct chamber_s x = {
        .gas = {
            .mol_ratio_co2 = 0.97,
            .mol_ratio_h2o = 0.03,
            .static_temperature_k = 300.0,
            .mass_kg = 1.0
        },
        .volume_m3 = 0.1,
        .nozzle_max_flow_area_m2 = 0.02,
        .nozzle_open_ratio = 1.0
    };
    struct chamber_s y = {
        .gas = {
            .mol_ratio_n2 = 0.78,
            .mol_ratio_o2 = 0.21,
            .mol_ratio_ar = 0.01,
            .static_temperature_k = 300.0,
            .mass_kg = 1.0
        },
        .volume_m3 = 1.0,
    };
    FILE* file_mix = fopen("visualize/chamber_s_mix.txt", "w");
    FILE* file_flow = fopen("visualize/chamber_s_flow.txt", "w");
    for(size_t cycle = 0; cycle < 3'000; cycle++)
    {
        struct nozzle_flow_s nozzle_flow = flow(&x, &y);
        if(nozzle_flow.is_success)
        {
            mail_gas_mail(&nozzle_flow.gas_mail);
        }
        fprintf(
            file_mix,
            "%f " /*  1 */
            "%f " /*  2 */
            "%f " /*  3 */
            "%f " /*  4 */
            "%f " /*  5 */
            "%f " /*  6 */
            "%f " /*  7 */
            "%f " /*  8 */
            "%f " /*  9 */
            "%f " /* 10 */
            "%f " /* 11 */
            "%f " /* 12 */
            "%f " /* 13 */
            "%f " /* 14 */
            "%f " /* 15 */
            "%f " /* 16 */
            "%f " /* 17 */
            "%f " /* 18 */
            "%f " /* 19 */
            "%f " /* 20 */
            "%f " /* 21 */
            "\n"
            , /*  1 */ g_std_dt_s * cycle
            , /*  2 */ calc_mixed_cp_j_per_mol_k(&x.gas)
            , /*  3 */ calc_mixed_cp_j_per_mol_k(&y.gas)
            , /*  4 */ calc_mixed_cv_j_per_mol_k(&x.gas)
            , /*  5 */ calc_mixed_cv_j_per_mol_k(&y.gas)
            , /*  6 */ calc_total_cp_j_per_k(&x.gas)
            , /*  7 */ calc_total_cp_j_per_k(&y.gas)
            , /*  8 */ calc_total_cv_j_per_k(&x.gas)
            , /*  9 */ calc_total_cv_j_per_k(&y.gas)
            , /* 10 */ calc_moles(&x.gas)
            , /* 11 */ calc_moles(&y.gas)
            , /* 12 */ calc_mixed_molar_mass_kg_per_mol(&x.gas)
            , /* 13 */ calc_mixed_molar_mass_kg_per_mol(&y.gas)
            , /* 14 */ calc_mixed_gamma(&x.gas)
            , /* 15 */ calc_mixed_gamma(&y.gas)
            , /* 16 */ calc_specific_gas_constant_j_per_kg_k(&x.gas)
            , /* 17 */ calc_specific_gas_constant_j_per_kg_k(&y.gas)
            , /* 18 */ nozzle_flow.flow_field.mach
            , /* 19 */ nozzle_flow.flow_field.mass_flow_rate_kg_per_s
            , /* 20 */ nozzle_flow.flow_field.velocity_m_per_s
            , /* 21 */ nozzle_flow.flow_field.speed_of_sound_m_per_s
        );
        fprintf(
            file_flow,
            "%f " /*  1 */
            "%f " /*  2 */
            "%f " /*  3 */
            "%f " /*  4 */
            "%f " /*  5 */
            "%f " /*  6 */
            "%f " /*  7 */
            "%f " /*  8 */
            "%f " /*  9 */
            "%f " /* 10 */
            "%f " /* 11 */
            "%f " /* 12 */
            "%f " /* 13 */
            "%f " /* 14 */
            "%f " /* 15 */
            "%f " /* 16 */
            "%f " /* 17 */
            "%f " /* 18 */
            "%f " /* 19 */
            "%f " /* 20 */
            "%f " /* 21 */
            "%f " /* 22 */
            "%f " /* 23 */
            "\n"
            , /*  1 */ g_std_dt_s * cycle
            , /*  2 */ calc_bulk_flow_velocity_m_per_s(&x.gas)
            , /*  3 */ calc_bulk_flow_velocity_m_per_s(&y.gas)
            , /*  4 */ calc_bulk_speed_of_sound_m_per_s(&x.gas)
            , /*  5 */ calc_bulk_speed_of_sound_m_per_s(&y.gas)
            , /*  6 */ calc_bulk_mach(&x.gas)
            , /*  7 */ calc_bulk_mach(&y.gas)
            , /*  8 */ calc_max_bulk_momentum_kg_m_per_s(&x.gas)
            , /*  9 */ calc_max_bulk_momentum_kg_m_per_s(&y.gas)
            , /* 10 */ x.gas.momentum_kg_m_per_s
            , /* 11 */ y.gas.momentum_kg_m_per_s
            , /* 12 */ calc_static_pressure_pa(&x)
            , /* 13 */ calc_static_pressure_pa(&y)
            , /* 14 */ x.gas.static_temperature_k
            , /* 15 */ y.gas.static_temperature_k
            , /* 16 */ calc_total_pressure_pa(&x)
            , /* 17 */ calc_total_pressure_pa(&y)
            , /* 18 */ calc_total_temperature_k(&x)
            , /* 19 */ calc_total_temperature_k(&y)
            , /* 20 */ nozzle_flow.flow_field.mach
            , /* 21 */ nozzle_flow.flow_field.mass_flow_rate_kg_per_s
            , /* 22 */ nozzle_flow.flow_field.velocity_m_per_s
            , /* 23 */ nozzle_flow.flow_field.speed_of_sound_m_per_s
        );
    }
    fclose(file_mix);
    fclose(file_flow);
}
