constexpr double g_engine_piston_diameter_m = 0.059;
constexpr double g_engine_piston_crank_throw_length_m = 0.031;
constexpr double g_engine_piston_connecting_rod_length_m = 0.11;
constexpr double g_engine_piston_connecting_rod_mass_kg = 0.4;
constexpr double g_engine_piston_head_mass_density_kg_per_m3 = 7800.0;
constexpr double g_engine_piston_head_compression_height_m = 0.018;
constexpr double g_engine_piston_head_clearance_height_m = 0.007;
constexpr double g_engine_piston_dynamic_friction_coefficient_n_m_s_per_r = 0.020;
constexpr double g_engine_piston_static_friction_coefficient_n_m_s_per_r = 0.9;
constexpr double g_engine_ideal_chamber_volume_m3 = 2e-4;
constexpr double g_engine_ideal_nozzle_max_flow_area_m2 = 2.4e-3;
constexpr double g_engine_ideal_gas_momentum_damping_time_constant_s = 0.33e-3;

struct node_s g_engine_node[] = {
    [0] = {
        .type = g_is_source,
        .as.source = {
            .chamber = {
                .volume_m3 = 1e20,
                .nozzle_max_flow_area_m2 = g_engine_ideal_nozzle_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_ideal_gas_momentum_damping_time_constant_s,
            },
        },
        .next = {1}
    },
    [1] = {
        .type = g_is_throttle,
        .as.throttle = {
            .chamber = {
                .volume_m3 = g_engine_ideal_chamber_volume_m3,
                .nozzle_max_flow_area_m2 = 0.25 * g_engine_ideal_nozzle_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_ideal_gas_momentum_damping_time_constant_s,
            },
        },
        .next = {2}
    },
    [2] = {
        .type = g_is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = g_engine_ideal_chamber_volume_m3,
                .nozzle_max_flow_area_m2 = 1.2 * g_engine_ideal_nozzle_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_ideal_gas_momentum_damping_time_constant_s,
            },
            .valve = {
                .engage_r = -0.10 * g_std_pi_r,
                .ramp_r = 0.8 * g_std_pi_r,
            },
        },
        .next = {4}
    },
    [3] = {
        .type = g_is_injector,
        .as.injector = {
            .chamber = {
                .volume_m3 = g_engine_ideal_chamber_volume_m3,
                .nozzle_max_flow_area_m2 = 0.0015 * g_engine_ideal_nozzle_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_ideal_gas_momentum_damping_time_constant_s,
            },
            .nozzle_index = 2,
        },
        .next = {4}
    },
    [4] = {
        .type = g_is_piston,
        .as.piston = {
            .chamber = {
                .nozzle_max_flow_area_m2 = 0.45 * g_engine_ideal_nozzle_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_ideal_gas_momentum_damping_time_constant_s,
            },
            .valve = {
                .engage_r = 2.75 * g_std_pi_r,
                .ramp_r = 0.9 * g_std_pi_r,
            },
            .sparkplug = {
                .engage_r = 2.10 * g_std_pi_r,
                .on_r = 0.25 * g_std_pi_r,
            },
            .diameter_m = g_engine_piston_diameter_m,
            .theta_r = 0.0,
            .crank_throw_length_m = g_engine_piston_crank_throw_length_m,
            .connecting_rod_length_m = g_engine_piston_connecting_rod_length_m,
            .connecting_rod_mass_kg = g_engine_piston_connecting_rod_mass_kg,
            .head_mass_density_kg_per_m3 = g_engine_piston_head_mass_density_kg_per_m3,
            .head_compression_height_m = g_engine_piston_head_compression_height_m,
            .head_clearance_height_m = g_engine_piston_head_clearance_height_m,
            .dynamic_friction_coefficient_n_m_s_per_r = g_engine_piston_dynamic_friction_coefficient_n_m_s_per_r,
            .static_friction_coefficient_n_m_s_per_r = g_engine_piston_static_friction_coefficient_n_m_s_per_r,
        },
        .next = {5}
    },
    [5] = {
        .type = g_is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = g_engine_ideal_chamber_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_ideal_nozzle_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_ideal_gas_momentum_damping_time_constant_s,
            },
        },
        .next = {6}
    },
    [6] = {
        .type = g_is_eplenum,
        .as.eplenum = {
            .chamber = {
                .volume_m3 = g_engine_ideal_chamber_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_ideal_nozzle_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_ideal_gas_momentum_damping_time_constant_s,
            },
            .wave_index = 0,
            .pipe_length_m = 0.6,
        },
        .next = {7}
    },
    [7] = {
        .type = g_is_exhaust,
        .as.exhaust = {
            .chamber = {
                .volume_m3 = g_engine_ideal_chamber_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_ideal_nozzle_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_ideal_gas_momentum_damping_time_constant_s,
            },
        },
        .next = {8}
    },
    [8] = {
        .type = g_is_sink,
        .as.sink = {
            .chamber = {
                .volume_m3 = 1e20,
                .nozzle_max_flow_area_m2 = g_engine_ideal_nozzle_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_ideal_gas_momentum_damping_time_constant_s,
            },
        },
        .next = {}
    },
};

struct engine_s g_engine = {
    "twin",
    engine_is(g_engine_node),
    .crankshaft = {
        .mass_kg = 1.0,
        .radius_m = 0.04,
    },
    .flywheel = {
        .mass_kg = 1.25,
        .radius_m = 0.08,
    },
    .limiter = {
        .cutoff_angular_velocity_r_per_s = 1000.0,
        .relaxed_angular_velocity_r_per_s = 300.0,
    },
    .starter = {
        .rated_torque_n_m = 20.0,
        .no_load_angular_velocity_r_per_s = 500.0,
        .radius_m = 0.015,
    },
    .volume = 0.5,
    .no_throttle = 0.0,
    .low_throttle = 0.01,
    .mid_throttle = 0.10,
    .high_throttle = 0.99,
    .radial_spacing = 5.5,
};
