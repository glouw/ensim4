constexpr size_t g_sampler_max_channels = 8;
constexpr size_t g_sampler_max_samples = 16384;
constexpr double g_sampler_min_angular_velocity_r_per_s = g_std_four_pi_r * g_std_audio_sample_rate_hz / g_sampler_max_samples;

#define SAMPLES                                 \
    X(g_sample_volume_m3)                       \
    X(g_sample_sparkplug_voltage_v)             \
    X(g_sample_nozzle_area_m2)                  \
    X(g_sample_nozzle_mach)                     \
    X(g_sample_nozzle_static_density_kg_per_m3) \
    X(g_sample_nozzle_velocity_m_per_s)         \
    X(g_sample_nozzle_static_pressure_pa)       \
    X(g_sample_nozzle_mass_flow_rate_kg_per_s)  \
    X(g_sample_nozzle_speed_of_sound_m_per_s)   \
    X(g_sample_piston_gas_torque_n_m)           \
    X(g_sample_piston_inertia_torque_n_m)       \
    X(g_sample_static_pressure_pa)              \
    X(g_sample_total_pressure_pa)               \
    X(g_sample_static_temperature_k)            \
    X(g_sample_molar_air_fuel_ratio)            \
    X(g_sample_molar_fuel_ratio_c8h18)          \
    X(g_sample_molar_combusted_ratio_co2_h2o)   \
    X(g_sample_momentum_kg_m_per_s)             \
    X(g_sample_gamma)                           \

enum sample_name_e
{
#define X(N) N,
    SAMPLES
#undef X
    g_sample_name_e_size
};

constexpr char g_sample_name_string[][64] = {
#define X(N) #N,
    SAMPLES
#undef X
};

#undef SAMPLES

typedef double sampler_synth_t[g_synth_buffer_size];

struct sampler_s
{
    double channel[g_sampler_max_channels][g_sample_name_e_size][g_sampler_max_samples];
    double starter[g_sampler_max_samples];
    size_t index;
    size_t channel_index;
    size_t size;
};

static void
sample_starter(struct sampler_s* self, double starter_angular_velocity_r_per_s)
{
    self->starter[self->index] = starter_angular_velocity_r_per_s;
}

static void
sample_value(struct sampler_s* self, enum sample_name_e sample_name, double sample)
{
    self->channel[self->channel_index][sample_name][self->index] = sample;
}

static void
sample_channel(struct sampler_s* self, struct node_s* node, struct nozzle_flow_s* nozzle_flow, struct crankshaft_s* crankshaft)
{
    if(self->channel_index < g_sampler_max_channels)
    {
        sample_value(self, g_sample_static_pressure_pa, calc_static_pressure_pa(&node->as.chamber));
        sample_value(self, g_sample_total_pressure_pa, calc_total_pressure_pa(&node->as.chamber));
        sample_value(self, g_sample_static_temperature_k, node->as.chamber.gas.static_temperature_k);
        sample_value(self, g_sample_volume_m3, node->as.chamber.volume_m3);
        sample_value(self, g_sample_molar_air_fuel_ratio, calc_mol_air_fuel_ratio(&node->as.chamber.gas));
        sample_value(self, g_sample_molar_fuel_ratio_c8h18, node->as.chamber.gas.mol_ratio_c8h18);
        sample_value(self, g_sample_molar_combusted_ratio_co2_h2o, calc_mol_combusted_ratio(&node->as.chamber.gas));
        sample_value(self, g_sample_sparkplug_voltage_v, node->type == g_is_piston ? calc_sparkplug_voltage_v(&node->as.piston.sparkplug, crankshaft) : 0.0);
        sample_value(self, g_sample_piston_gas_torque_n_m, node->type == g_is_piston ? calc_piston_gas_torque_n_m(&node->as.piston, crankshaft) : 0.0);
        sample_value(self, g_sample_piston_inertia_torque_n_m, node->type == g_is_piston ? calc_piston_inertia_torque_n_m(&node->as.piston, crankshaft) : 0.0);
        sample_value(self, g_sample_nozzle_area_m2, nozzle_flow->area_m2);
        sample_value(self, g_sample_nozzle_mach, nozzle_flow->flow_field.mach);
        sample_value(self, g_sample_nozzle_static_density_kg_per_m3, nozzle_flow->flow_field.static_density_kg_per_m3);
        sample_value(self, g_sample_nozzle_velocity_m_per_s, nozzle_flow->flow_field.velocity_m_per_s);
        sample_value(self, g_sample_nozzle_static_pressure_pa, nozzle_flow->flow_field.static_pressure_pa);
        sample_value(self, g_sample_nozzle_mass_flow_rate_kg_per_s, nozzle_flow->flow_field.mass_flow_rate_kg_per_s);
        sample_value(self, g_sample_nozzle_speed_of_sound_m_per_s, nozzle_flow->flow_field.speed_of_sound_m_per_s);
        sample_value(self, g_sample_gamma, calc_mixed_gamma(&node->as.chamber.gas));
        sample_value(self, g_sample_momentum_kg_m_per_s, node->as.chamber.gas.momentum_kg_m_per_s);
        self->channel_index++;
    }
}

static void
clear_channel_sampler(struct sampler_s* self)
{
    clear(self->channel);
}

static void
reset_sampler_channel(struct sampler_s* self)
{
    self->channel_index = 0;
}

static const char*
till_underscore(const char* string)
{
    return strchr(string, '_') + 1;
}

static const char*
skip_sample_namespace(const char* string)
{
    return till_underscore(till_underscore(string));
}
