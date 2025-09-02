static constexpr double g_engine_1_cyl_intake_max_flow_area_m2 = 5.0e-4;
static constexpr double g_engine_1_cyl_exhaust_max_flow_area_m2 = 8.0e-4;
static constexpr double g_engine_1_cyl_throttle_max_flow_area_m2 = g_engine_1_cyl_intake_max_flow_area_m2 / 8.0;
static constexpr double g_engine_1_cyl_injector_max_flow_area_m2 = g_engine_1_cyl_intake_max_flow_area_m2 / 128.0;
static constexpr double g_engine_1_cyl_valve_engage_offset_r = -1.0;
static constexpr double g_engine_1_cyl_sparkplug_engage_offset_r = 0.25;
static constexpr double g_engine_1_cyl_sparkplug_on_theta_r = (1.0 / 32.0) * g_std_four_pi_r;
static constexpr double g_engine_1_cyl_0_theta_r = 0.0;
static constexpr double g_engine_1_cyl_gas_momentum_damping_time_constant_s = 0.5e-3;

static struct node_s g_node_1_cyl[] = {
    [0] = {
        .type = g_is_source,
        .as.source = {
            .chamber = {
                .volume_m3 = g_chamber_ambient_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_1_cyl_intake_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_1_cyl_gas_momentum_damping_time_constant_s,
            },
        },
        .next = {1}
    },
    [1] = {
        .type = g_is_throttle,
        .as.throttle = {
            .chamber = {
                .volume_m3 = 1e-4,
                .nozzle_max_flow_area_m2 = g_engine_1_cyl_throttle_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_1_cyl_gas_momentum_damping_time_constant_s,
            },
        },
        .next = {2}
    },
    [2] = {
        .type = g_is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 3e-4,
                .nozzle_max_flow_area_m2 = g_engine_1_cyl_intake_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_1_cyl_gas_momentum_damping_time_constant_s,
            },
            .valve = {
                .engage_r = g_engine_1_cyl_0_theta_r + g_engine_intake_stroke_theta_r + g_engine_1_cyl_valve_engage_offset_r,
                .ramp_r = g_engine_stroke_ratio * g_std_four_pi_r,
            },
        },
        .next = {4}
    },
    [3] = {
        .type = g_is_injector,
        .as.injector = {
            .chamber = {
                .volume_m3 = 1e-4,
                .nozzle_max_flow_area_m2 = g_engine_1_cyl_injector_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_1_cyl_gas_momentum_damping_time_constant_s,
            },
            .nozzle_index = 2,
        },
        .next = {4}
    },
    [4] = {
        .type = g_is_piston,
        .as.piston = {
            .chamber = {
                .nozzle_max_flow_area_m2 = g_engine_1_cyl_exhaust_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_1_cyl_gas_momentum_damping_time_constant_s,
            },
            .valve = {
                .engage_r = g_engine_1_cyl_0_theta_r + g_engine_exhaust_stroke_theta_r + g_engine_1_cyl_valve_engage_offset_r,
                .ramp_r = g_engine_stroke_ratio * g_std_four_pi_r,
            },
            .sparkplug = {
                .engage_r = g_engine_1_cyl_0_theta_r + g_engine_power_stroke_theta_r + g_engine_1_cyl_sparkplug_engage_offset_r,
                .on_r = g_engine_1_cyl_sparkplug_on_theta_r,
            },
            .diameter_m = 0.057,
            .theta_r = g_engine_1_cyl_0_theta_r,
            .crank_throw_length_m = 0.031,
            .connecting_rod_length_m = 0.11,
            .connecting_rod_mass_kg = 0.4,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.013,
            .head_clearance_height_m = 0.007,
            .dynamic_friction_coefficient_n_m_s_per_r = 0.015,
            .static_friction_coefficient_n_m_s_per_r = 2.0,
        },
        .next = {5}
    },
    [5] = {
        .type = g_is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 1.5e-4,
                .nozzle_max_flow_area_m2 = g_engine_1_cyl_exhaust_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_1_cyl_gas_momentum_damping_time_constant_s,
            },
        },
        .next = {6}
    },
    [6] = {
        .type = g_is_eplenum,
        .as.eplenum = {
            .chamber = {
                .volume_m3 = 3e-4,
                .nozzle_max_flow_area_m2 = g_engine_1_cyl_exhaust_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_1_cyl_gas_momentum_damping_time_constant_s,
            },
            .wave_index = 0,
        },
        .next = {7}
    },
    [7] = {
        .type = g_is_exhaust,
        .as.exhaust = {
            .chamber = {
                .volume_m3 = 6e-4,
                .nozzle_max_flow_area_m2 = g_engine_1_cyl_exhaust_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_1_cyl_gas_momentum_damping_time_constant_s,
            },
        },
        .next = {8}
    },
    [8] = {
        .type = g_is_sink,
        .as.sink = {
            .chamber = {
                .volume_m3 = g_chamber_ambient_volume_m3,
                .gas_momentum_damping_time_constant_s = g_engine_1_cyl_gas_momentum_damping_time_constant_s,
            },
        },
        .next = {}
    },
};

static struct engine_s g_engine_1_cyl = {
    "175cc",
    engine_is(g_node_1_cyl),
    .crankshaft = {
        .mass_kg = 1.0,
        .radius_m = 0.03,
    },
    .flywheel = {
        .mass_kg = 2.0,
        .radius_m = 0.1,
    },
    .limiter = {
        .cutoff_angular_velocity_r_per_s = 900.0,
        .relaxed_angular_velocity_r_per_s = 100.0,
    },
    .starter = {
        .rated_torque_n_m = 30.0,
        .no_load_angular_velocity_r_per_s = 900.0,
        .radius_m = 0.015,
    },
    .envelope = {
        .max_value = 0.5,
        .limiter = 1000.0,
    },
    .nozzle_total_pressure_deadband_pa = 1000.0,
};
