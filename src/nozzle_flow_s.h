struct nozzle_flow_field_s
{
    double mach;
    double velocity_m_per_s;
    double mass_flow_rate_kg_per_s;
    double speed_of_sound_m_per_s;
    double static_density_kg_per_m3;
    double static_pressure_pa;
};

struct nozzle_flow_s
{
    double area_m2;
    struct nozzle_flow_field_s flow_field;
    struct gas_mail_s gas_mail;
    bool is_success;
};

[[nodiscard("too computationally expensive to ignore return value")]]
static struct nozzle_flow_s
flow(struct chamber_s* x, struct chamber_s* y)
{
    double nozzle_flow_area_m2 = calc_nozzle_flow_area_m2(x);
    if(nozzle_flow_area_m2 > 0.0)
    {
        double direction = 1.0;
        if(calc_total_pressure_pa(x) < calc_total_pressure_pa(y))
        {
            swap(x, y);
            direction = -1.0;
        }
        double nozzle_mach = calc_nozzle_mach(x, y);
        if(nozzle_mach > 0.0)
        {
            double nozzle_flow_velocity_m_per_s = calc_nozzle_flow_velocity_m_per_s(x, nozzle_mach);
            double nozzle_mass_flow_rate_kg_per_s = calc_nozzle_mass_flow_rate_kg_per_s(x, nozzle_flow_area_m2, nozzle_mach);
            double nozzle_speed_of_sound_m_per_s = calc_nozzle_speed_of_sound_m_per_s(x, nozzle_mach, nozzle_flow_velocity_m_per_s);
            double nozzle_static_density_kg_per_m3 = calc_nozzle_static_density_kg_per_m3(nozzle_mass_flow_rate_kg_per_s, nozzle_flow_area_m2, nozzle_flow_velocity_m_per_s);
            double nozzle_static_pressure_pa = calc_nozzle_static_pressure_pa(x, nozzle_mach);
            double mass_flowed_kg = nozzle_mass_flow_rate_kg_per_s * g_std_dt_s;
            double momentum_transferred_kg = mass_flowed_kg * nozzle_flow_velocity_m_per_s;
            return (struct nozzle_flow_s) {
                .area_m2 = nozzle_flow_area_m2,
                .flow_field.mach = direction * nozzle_mach,
                .flow_field.velocity_m_per_s = direction * nozzle_flow_velocity_m_per_s,
                .flow_field.mass_flow_rate_kg_per_s = direction * nozzle_mass_flow_rate_kg_per_s,
                .flow_field.speed_of_sound_m_per_s = nozzle_speed_of_sound_m_per_s,
                .flow_field.static_density_kg_per_m3 = nozzle_static_density_kg_per_m3,
                .flow_field.static_pressure_pa = nozzle_static_pressure_pa,
                .gas_mail = {
                    .gas = {
                        .mol_ratio_c8h18 = x->gas.mol_ratio_c8h18,
                        .mol_ratio_o2 = x->gas.mol_ratio_o2,
                        .mol_ratio_n2 = x->gas.mol_ratio_n2,
                        .mol_ratio_ar = x->gas.mol_ratio_ar,
                        .mol_ratio_co2 = x->gas.mol_ratio_co2,
                        .mol_ratio_h2o = x->gas.mol_ratio_h2o,
                        .static_temperature_k = x->gas.static_temperature_k,
                        .mass_kg = mass_flowed_kg,
                        .momentum_kg_m_per_s = momentum_transferred_kg,
                    },
                    .x = x,
                    .y = y,
                },
                .is_success = true,
            };
        }
    }
    return (struct nozzle_flow_s) {
        .flow_field.static_density_kg_per_m3 = calc_bulk_static_density_kg_per_m3(x),
        .flow_field.static_pressure_pa = calc_static_pressure_pa(x),
    };
}
