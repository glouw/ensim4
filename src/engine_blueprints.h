#define source_d(...) {                                                                            \
    .type = g_is_source,                                                                           \
    .as.source = {                                                                                 \
        .chamber = {                                                                               \
            .volume_m3 = g_engine_source_sink_volume_m3,                                           \
            .nozzle_max_flow_area_m2 = g_engine_source_max_flow_area_m2,                           \
            .gas_momentum_damping_time_constant_s = g_engine_gas_momentum_damping_time_constant_s, \
        },                                                                                         \
    },                                                                                             \
    .next = __VA_ARGS__                                                                            \
}

#define afilter_d(...) {                                                                           \
    .type = g_is_afilter,                                                                          \
    .as.afilter = {                                                                                \
        .chamber = {                                                                               \
            .volume_m3 = g_engine_afilter_volume_m3,                                               \
            .nozzle_max_flow_area_m2 = g_engine_afilter_max_flow_area_m2,                          \
            .gas_momentum_damping_time_constant_s = g_engine_gas_momentum_damping_time_constant_s, \
        },                                                                                         \
    },                                                                                             \
    .next = __VA_ARGS__                                                                            \
}

#define throttle_d(...) {                                                                          \
    .type = g_is_throttle,                                                                         \
    .as.throttle = {                                                                               \
        .chamber = {                                                                               \
            .volume_m3 = g_engine_throttle_volume_m3,                                              \
            .nozzle_max_flow_area_m2 = g_engine_throttle_max_flow_area_m2,                         \
            .gas_momentum_damping_time_constant_s = g_engine_gas_momentum_damping_time_constant_s, \
        },                                                                                         \
    },                                                                                             \
    .next = __VA_ARGS__                                                                            \
}

#define irunner_d(_theta_r, ...) {                                                                 \
    .type = g_is_irunner,                                                                          \
    .as.irunner = {                                                                                \
        .chamber = {                                                                               \
            .volume_m3 = g_engine_irunner_volume_m3,                                               \
            .nozzle_max_flow_area_m2 = g_engine_irunner_max_flow_area_m2,                          \
            .gas_momentum_damping_time_constant_s = g_engine_gas_momentum_damping_time_constant_s, \
        },                                                                                         \
        .valve = {                                                                                 \
            .engage_r = _theta_r + g_engine_irunner_valve_engage_r,                                \
            .ramp_r = g_engine_irunner_valve_ramp_r,                                               \
        },                                                                                         \
    },                                                                                             \
    .next = __VA_ARGS__                                                                            \
}

#define injector_d(_nozzle_index, ...) {                                                           \
    .type = g_is_injector,                                                                         \
    .as.injector = {                                                                               \
        .chamber = {                                                                               \
            .volume_m3 = g_engine_injector_volume_m3,                                              \
            .nozzle_max_flow_area_m2 = g_engine_injector_max_flow_area_m2,                         \
            .gas_momentum_damping_time_constant_s = g_engine_gas_momentum_damping_time_constant_s, \
        },                                                                                         \
        .nozzle_index = _nozzle_index,                                                             \
    },                                                                                             \
    .next = __VA_ARGS__                                                                            \
}

#define piston_d(_theta_r, ...) {                                                                  \
    .type = g_is_piston,                                                                           \
    .as.piston = {                                                                                 \
        .chamber = {                                                                               \
            .nozzle_max_flow_area_m2 = g_engine_piston_max_flow_area_m2,                           \
            .gas_momentum_damping_time_constant_s = g_engine_gas_momentum_damping_time_constant_s, \
        },                                                                                         \
        .valve = {                                                                                 \
            .engage_r = _theta_r + g_engine_piston_valve_engage_r,                                 \
            .ramp_r = g_engine_piston_valve_ramp_r,                                                \
        },                                                                                         \
        .sparkplug = {                                                                             \
            .engage_r = _theta_r + g_engine_sparkplug_engage_r,                                    \
            .on_r = g_engine_sparkplug_on_r,                                                       \
        },                                                                                         \
        .diameter_m = g_engine_piston_diameter_m,                                                  \
        .theta_r = -_theta_r,                                                                      \
        .crank_throw_length_m = g_engine_piston_crank_throw_length_m,                              \
        .connecting_rod_length_m = g_engine_piston_connecting_rod_length_m,                        \
        .connecting_rod_mass_kg = g_engine_piston_connecting_rod_mass_kg,                          \
        .head_mass_density_kg_per_m3 = g_engine_piston_head_mass_density_kg_per_m3,                \
        .head_compression_height_m = g_engine_piston_head_compression_height_m,                    \
        .head_clearance_height_m = g_engine_piston_head_clearance_height_m,                        \
        .dynamic_friction_n_m_s_per_r = g_engine_piston_dynamic_friction_n_m_s_per_r,              \
        .static_friction_n_m_s_per_r = g_engine_piston_static_friction_n_m_s_per_r,                \
    },                                                                                             \
    .next = __VA_ARGS__                                                                            \
}

#define erunner_d(...) {                                                                           \
    .type = g_is_erunner,                                                                          \
    .as.erunner = {                                                                                \
        .chamber = {                                                                               \
            .volume_m3 = g_engine_erunner_volume_m3,                                               \
            .nozzle_max_flow_area_m2 = g_engine_erunner_max_flow_area_m2,                          \
            .gas_momentum_damping_time_constant_s = g_engine_gas_momentum_damping_time_constant_s, \
        },                                                                                         \
    },                                                                                             \
    .next = __VA_ARGS__                                                                            \
}

#define eplenum_d(_wave_index, ...) {                                                              \
    .type = g_is_eplenum,                                                                          \
    .as.eplenum = {                                                                                \
        .chamber = {                                                                               \
            .volume_m3 = g_engine_eplenum_volume_m3,                                               \
            .nozzle_max_flow_area_m2 = g_engine_eplenum_max_flow_area_m2,                          \
            .gas_momentum_damping_time_constant_s = g_engine_gas_momentum_damping_time_constant_s, \
        },                                                                                         \
        .wave_index = _wave_index,                                                                 \
        .pipe_length_m = g_engine_eplenum_wave_pipe_length_m,                                      \
        .mic_position_ratio = g_engine_mic_position_ratio,                                         \
        .velocity_low_pass_cutoff_frequency_hz = g_engine_velocity_low_pass_cutoff_frequency_hz,   \
    },                                                                                             \
    .next = __VA_ARGS__                                                                            \
}

#define exhaust_d(...) {                                                                           \
    .type = g_is_exhaust,                                                                          \
    .as.exhaust = {                                                                                \
        .chamber = {                                                                               \
            .volume_m3 = g_engine_exhaust_volume_m3,                                               \
            .nozzle_max_flow_area_m2 = g_engine_exhaust_max_flow_area_m2,                          \
            .gas_momentum_damping_time_constant_s = g_engine_gas_momentum_damping_time_constant_s, \
        },                                                                                         \
    },                                                                                             \
    .next = __VA_ARGS__                                                                            \
}

#define sink_d() {                                                                                 \
    .type = g_is_sink,                                                                             \
    .as.sink = {                                                                                   \
        .chamber = {                                                                               \
            .volume_m3 = g_engine_source_sink_volume_m3,                                           \
            .gas_momentum_damping_time_constant_s = g_engine_gas_momentum_damping_time_constant_s, \
        },                                                                                         \
    },                                                                                             \
    .next = {}                                                                                     \
}
