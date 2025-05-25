struct nozzle_flow_s
{
    double area_m2;
    double mach;
    double velocity_m_per_s;
    double mass_flow_rate_kg_per_s;
    double speed_of_sound_m_per_s;
    struct gas_mail_s gas_mail;
};

static struct nozzle_flow_s
flow(const struct chamber_s* x, const struct chamber_s* y)
{
    double nozzle_flow_area_m2 = calc_nozzle_flow_area_m2(x);
    if(nozzle_flow_area_m2 > 0.0)
    {
        double direction = 1.0;
        if(calc_total_pressure_pa(x) < calc_total_pressure_pa(y))
        {
            const struct chamber_s* copy = x;
            x = y;
            y = copy;
            direction = -1.0;
        }
        double nozzle_mach = calc_nozzle_mach(x, y);
        double nozzle_flow_velocity_m_per_s = calc_nozzle_flow_velocity_m_per_s(x, y);
        double nozzle_mass_flow_rate_kg_per_s = calc_nozzle_mass_flow_rate_kg_per_s(x, y, nozzle_flow_area_m2);
        double nozzle_speed_of_sound_m_per_s = calc_nozzle_speed_of_sound_m_per_s(x, y);
        double mass_flowed_kg = nozzle_mass_flow_rate_kg_per_s * std_dt_s;
        double momentum_transferred_kg = mass_flowed_kg * nozzle_flow_velocity_m_per_s;
        struct gas_mail_s gas_mail = {
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
            .x = (struct chamber_s*) x,
            .y = (struct chamber_s*) y,
        };
        return (struct nozzle_flow_s) {
            .area_m2 = nozzle_flow_area_m2,
            .mach = direction * nozzle_mach,
            .velocity_m_per_s = direction * nozzle_flow_velocity_m_per_s,
            .mass_flow_rate_kg_per_s = direction * nozzle_mass_flow_rate_kg_per_s,
            .speed_of_sound_m_per_s = nozzle_speed_of_sound_m_per_s,
            .gas_mail = gas_mail,
        };
    }
    return (struct nozzle_flow_s) {};
}
