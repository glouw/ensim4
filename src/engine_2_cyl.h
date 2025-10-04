constexpr char g_engine_name[] = "twin";
/* piston spec */
constexpr double g_engine_piston_diameter_m                    = 0.059;
constexpr double g_engine_piston_crank_throw_length_m          = 0.031;
constexpr double g_engine_piston_connecting_rod_length_m       = 0.11;
constexpr double g_engine_piston_connecting_rod_mass_kg        = 0.40;
constexpr double g_engine_piston_head_mass_density_kg_per_m3   = 7800.0;
constexpr double g_engine_piston_head_compression_height_m     = 0.018;
constexpr double g_engine_piston_head_clearance_height_m       = 0.007;
constexpr double g_engine_piston_dynamic_friction_n_m_s_per_r  = 0.020;
constexpr double g_engine_piston_static_friction_n_m_s_per_r   = 0.90;
constexpr double g_engine_gas_momentum_damping_time_constant_s = 0.33e-3;
/* wave spec */
constexpr double g_engine_eplenum_wave_pipe_length_m = 0.6;
/* chamber spec */
constexpr double g_engine_chamber_volume_m3  = 2.00e-4;
constexpr double g_engine_throttle_volume_m3 = 0.10 * g_engine_chamber_volume_m3;
constexpr double g_engine_irunner_volume_m3  = 0.50 * g_engine_chamber_volume_m3;
constexpr double g_engine_injector_volume_m3 = 0.02 * g_engine_chamber_volume_m3;
constexpr double g_engine_erunner_volume_m3  = 0.40 * g_engine_chamber_volume_m3;
constexpr double g_engine_eplenum_volume_m3  = 1.50 * g_engine_chamber_volume_m3;
constexpr double g_engine_exhaust_volume_m3  = 0.80 * g_engine_chamber_volume_m3;
/* flow spec */
constexpr double g_engine_max_flow_area_m2          = 2.40e-3;
constexpr double g_engine_source_max_flow_area_m2   = 0.300 * g_engine_max_flow_area_m2;
constexpr double g_engine_throttle_max_flow_area_m2 = 0.250 * g_engine_max_flow_area_m2;
constexpr double g_engine_irunner_max_flow_area_m2  = 0.600 * g_engine_max_flow_area_m2;
constexpr double g_engine_injector_max_flow_area_m2 = 0.005 * g_engine_max_flow_area_m2;
constexpr double g_engine_piston_max_flow_area_m2   = 0.500 * g_engine_max_flow_area_m2;
constexpr double g_engine_erunner_max_flow_area_m2  = 0.450 * g_engine_max_flow_area_m2;
constexpr double g_engine_eplenum_max_flow_area_m2  = 0.800 * g_engine_max_flow_area_m2;
constexpr double g_engine_exhaust_max_flow_area_m2  = 0.900 * g_engine_max_flow_area_m2;
/* belt timings */
constexpr double g_engine_piston_0_theta_r     = 0.00 * g_std_pi_r;
constexpr double g_engine_piston_1_theta_r     = 1.00 * g_std_pi_r;
constexpr double g_engine_irunner_engage_r     = -0.15 * g_std_pi_r;
constexpr double g_engine_irunner_0_engage_r   = g_engine_piston_0_theta_r + g_engine_irunner_engage_r;
constexpr double g_engine_irunner_1_engage_r   = g_engine_piston_1_theta_r + g_engine_irunner_engage_r;
constexpr double g_engine_piston_engage_r      = 2.70 * g_std_pi_r;
constexpr double g_engine_piston_0_engage_r    = g_engine_piston_0_theta_r + g_engine_piston_engage_r;
constexpr double g_engine_piston_1_engage_r    = g_engine_piston_1_theta_r + g_engine_piston_engage_r;
constexpr double g_engine_sparkplug_engage_r   = 2.05 * g_std_pi_r;
constexpr double g_engine_sparkplug_0_engage_r = g_engine_piston_0_theta_r + g_engine_sparkplug_engage_r;
constexpr double g_engine_sparkplug_1_engage_r = g_engine_piston_1_theta_r + g_engine_sparkplug_engage_r;
constexpr double g_engine_irunner_ramp_r       = 1.00 * g_std_pi_r;
constexpr double g_engine_piston_ramp_r        = 0.95 * g_std_pi_r;
constexpr double g_engine_sparkplug_on_r       = 0.25 * g_std_pi_r;
/* throttle spec */
constexpr double g_engine_no_throttle   = 0.000;
constexpr double g_engine_low_throttle  = 0.001;
constexpr double g_engine_mid_throttle  = 0.050;
constexpr double g_engine_high_throttle = 1.000;
/* accessory spec */
constexpr double g_engine_crankshaft_mass_kg       = 1.0;
constexpr double g_engine_crankshaft_radius_m      = 0.04;
constexpr double g_engine_flywheel_mass_kg         = 1.25;
constexpr double g_engine_flywheel_radius_m        = 0.08;
constexpr double g_engine_limiter_cutoff_r_per_s   = 1500.0;
constexpr double g_engine_limiter_relaxed_r_per_s  = 300.0;
constexpr double e_engine_starter_rated_torque_n_m = 20.0;
constexpr double g_engine_starter_no_load_r_per_s  = 500.0;
constexpr double g_engine_starter_radius_m         = 0.015;
/* misc spec */
constexpr double g_engine_sound_volume   = 1.0;
constexpr double g_engine_radial_spacing = 3.0;
constexpr double g_engine_source_sink_volume_m3 = 1.00e20;

struct node_s g_engine_node[] = {
    [0] = {
        .type = g_is_source,
        .as.source = {
            .chamber = {
                .volume_m3 = g_engine_source_sink_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_source_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_gas_momentum_damping_time_constant_s,
            },
        },
        .next = {1}
    },
    [1] = {
        .type = g_is_throttle,
        .as.throttle = {
            .chamber = {
                .volume_m3 = g_engine_throttle_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_throttle_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_gas_momentum_damping_time_constant_s,
            },
        },
        .next = {2, 6}
    },
    [2] = {
        .type = g_is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = g_engine_irunner_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_irunner_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_gas_momentum_damping_time_constant_s,
            },
            .valve = {
                .engage_r = g_engine_irunner_0_engage_r,
                .ramp_r = g_engine_irunner_ramp_r,
            },
        },
        .next = {4}
    },
    [3] = {
        .type = g_is_injector,
        .as.injector = {
            .chamber = {
                .volume_m3 = g_engine_injector_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_injector_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_gas_momentum_damping_time_constant_s,
            },
            .nozzle_index = 2,
        },
        .next = {4}
    },
    [4] = {
        .type = g_is_piston,
        .as.piston = {
            .chamber = {
                .nozzle_max_flow_area_m2 = g_engine_piston_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_gas_momentum_damping_time_constant_s,
            },
            .valve = {
                .engage_r = g_engine_piston_0_engage_r,
                .ramp_r = g_engine_piston_ramp_r,
            },
            .sparkplug = {
                .engage_r = g_engine_sparkplug_0_engage_r,
                .on_r = g_engine_sparkplug_on_r,
            },
            .diameter_m = g_engine_piston_diameter_m,
            .theta_r = g_engine_piston_0_theta_r,
            .crank_throw_length_m = g_engine_piston_crank_throw_length_m,
            .connecting_rod_length_m = g_engine_piston_connecting_rod_length_m,
            .connecting_rod_mass_kg = g_engine_piston_connecting_rod_mass_kg,
            .head_mass_density_kg_per_m3 = g_engine_piston_head_mass_density_kg_per_m3,
            .head_compression_height_m = g_engine_piston_head_compression_height_m,
            .head_clearance_height_m = g_engine_piston_head_clearance_height_m,
            .dynamic_friction_n_m_s_per_r = g_engine_piston_dynamic_friction_n_m_s_per_r,
            .static_friction_n_m_s_per_r = g_engine_piston_static_friction_n_m_s_per_r,
        },
        .next = {5}
    },
    [5] = {
        .type = g_is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = g_engine_erunner_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_erunner_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_gas_momentum_damping_time_constant_s,
            },
        },
        .next = {10}
    },
    [6] = {
        .type = g_is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = g_engine_irunner_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_irunner_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_gas_momentum_damping_time_constant_s,
            },
            .valve = {
                .engage_r = g_engine_irunner_1_engage_r,
                .ramp_r = g_engine_irunner_ramp_r,
            },
        },
        .next = {8}
    },
    [7] = {
        .type = g_is_injector,
        .as.injector = {
            .chamber = {
                .volume_m3 = g_engine_injector_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_injector_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_gas_momentum_damping_time_constant_s,
            },
            .nozzle_index = 6,
        },
        .next = {8}
    },
    [8] = {
        .type = g_is_piston,
        .as.piston = {
            .chamber = {
                .nozzle_max_flow_area_m2 = g_engine_piston_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_gas_momentum_damping_time_constant_s,
            },
            .valve = {
                .engage_r = g_engine_piston_1_engage_r,
                .ramp_r = g_engine_piston_ramp_r,
            },
            .sparkplug = {
                .engage_r = g_engine_sparkplug_1_engage_r,
                .on_r = g_engine_sparkplug_on_r,
            },
            .diameter_m = g_engine_piston_diameter_m,
            .theta_r = g_engine_piston_1_theta_r,
            .crank_throw_length_m = g_engine_piston_crank_throw_length_m,
            .connecting_rod_length_m = g_engine_piston_connecting_rod_length_m,
            .connecting_rod_mass_kg = g_engine_piston_connecting_rod_mass_kg,
            .head_mass_density_kg_per_m3 = g_engine_piston_head_mass_density_kg_per_m3,
            .head_compression_height_m = g_engine_piston_head_compression_height_m,
            .head_clearance_height_m = g_engine_piston_head_clearance_height_m,
            .dynamic_friction_n_m_s_per_r = g_engine_piston_dynamic_friction_n_m_s_per_r,
            .static_friction_n_m_s_per_r = g_engine_piston_static_friction_n_m_s_per_r,
        },
        .next = {9}
    },
    [9] = {
        .type = g_is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = g_engine_erunner_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_erunner_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_gas_momentum_damping_time_constant_s,
            },
        },
        .next = {10}
    },
    [10] = {
        .type = g_is_eplenum,
        .as.eplenum = {
            .chamber = {
                .volume_m3 = g_engine_eplenum_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_eplenum_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_gas_momentum_damping_time_constant_s,
            },
            .wave_index = 0,
            .pipe_length_m = g_engine_eplenum_wave_pipe_length_m,
        },
        .next = {11}
    },
    [11] = {
        .type = g_is_exhaust,
        .as.exhaust = {
            .chamber = {
                .volume_m3 = g_engine_exhaust_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_exhaust_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_gas_momentum_damping_time_constant_s,
            },
        },
        .next = {12}
    },
    [12] = {
        .type = g_is_sink,
        .as.sink = {
            .chamber = {
                .volume_m3 = g_engine_source_sink_volume_m3,
                .gas_momentum_damping_time_constant_s = g_engine_gas_momentum_damping_time_constant_s,
            },
        },
        .next = {}
    },
};

struct engine_s g_engine = {
    .name = g_engine_name,
    engine_is(g_engine_node),
    .crankshaft = {
        .mass_kg = g_engine_crankshaft_mass_kg,
        .radius_m = g_engine_crankshaft_radius_m,
    },
    .flywheel = {
        .mass_kg = g_engine_flywheel_mass_kg,
        .radius_m = g_engine_flywheel_radius_m,
    },
    .limiter = {
        .cutoff_angular_velocity_r_per_s = g_engine_limiter_cutoff_r_per_s,
        .relaxed_angular_velocity_r_per_s = g_engine_limiter_relaxed_r_per_s,
    },
    .starter = {
        .rated_torque_n_m = e_engine_starter_rated_torque_n_m,
        .no_load_angular_velocity_r_per_s = g_engine_starter_no_load_r_per_s,
        .radius_m = g_engine_starter_radius_m,
    },
    .volume = g_engine_sound_volume,
    .no_throttle = g_engine_no_throttle,
    .low_throttle = g_engine_low_throttle,
    .mid_throttle = g_engine_mid_throttle,
    .high_throttle = g_engine_high_throttle,
    .radial_spacing = g_engine_radial_spacing,
};
