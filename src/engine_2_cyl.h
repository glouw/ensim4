static constexpr double g_engine_2_cyl_channel_count = 2.0;
static constexpr double g_engine_2_cyl_intake_channel_max_flow_area_m2 = 2.5e-3;
static constexpr double g_engine_2_cyl_exhaust_channel_max_flow_area_m2 = 2.9e-3;
static constexpr double g_engine_2_cyl_exhaust_junction_max_flow_area_m2 = g_engine_2_cyl_channel_count * g_engine_2_cyl_exhaust_channel_max_flow_area_m2;
static constexpr double g_engine_2_cyl_throttle_max_flow_area_m2 = g_engine_2_cyl_intake_channel_max_flow_area_m2 / 32.0;
static constexpr double g_engine_2_cyl_injector_max_flow_area_m2 = g_engine_2_cyl_intake_channel_max_flow_area_m2 / 128.0;
static constexpr double g_engine_2_cyl_sparkplug_on_theta_r = (2.0 / 32.0) * g_std_four_pi_r;
static constexpr double g_engine_2_cyl_0_theta_r = (0.0 / 4.0) * g_std_four_pi_r;
static constexpr double g_engine_2_cyl_1_theta_r = (1.0 / 4.0) * g_std_four_pi_r;
static constexpr double g_engine_2_cyl_channel_volume_m3 = 1.2e-4;
static constexpr double g_engine_2_cyl_junction_volume_m3 = g_engine_2_cyl_channel_volume_m3 * g_engine_2_cyl_channel_count;
static constexpr double g_engine_2_cyl_valve_ramp_r = (0.85 / 4.0) * g_std_four_pi_r;
static constexpr double g_engine_2_cyl_intake_stroke_theta_r = g_engine_intake_stroke_theta_r - (0.2 / 4.0) * g_std_four_pi_r;
static constexpr double g_engine_2_cyl_exhaust_stroke_theta_r = g_engine_exhaust_stroke_theta_r - (0.2 / 4.0) * g_std_four_pi_r;
static constexpr double g_engine_2_cyl_power_stroke_theta_r = g_engine_power_stroke_theta_r + (0.1 / 4.0) * g_std_four_pi_r;
static constexpr double g_engine_2_cyl_piston_dynamic_friction_coefficient_n_m_s_per_r = 0.01;
static constexpr double g_engine_2_cyl_piston_static_friction_coefficient_n_m_s_per_r = 0.9;
static constexpr double g_engine_2_cyl_piston_diameter_m = 0.057;
static constexpr double g_engine_2_cyl_piston_crank_throw_length_m = 0.031;
static constexpr double g_engine_2_cyl_piston_connecting_rod_length_m = 0.11;
static constexpr double g_engine_2_cyl_piston_connecting_rod_mass_kg = 0.4;
static constexpr double g_engine_2_cyl_piston_head_mass_density_kg_per_m3 = 7800.0;
static constexpr double g_engine_2_cyl_piston_head_compression_height_m = 0.018;
static constexpr double g_engine_2_cyl_piston_head_clearance_height_m = 0.007;

static struct node_s g_node_2_cyl[] = {
    [0] = {
        .type = g_is_source,
        .as.source = {
            .chamber = {
                .volume_m3 = g_chamber_ambient_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_2_cyl_intake_channel_max_flow_area_m2,
            },
        },
        .next = {1}
    },
    [1] = {
        .type = g_is_throttle,
        .as.throttle = {
            .chamber = {
                .volume_m3 = g_engine_2_cyl_channel_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_2_cyl_throttle_max_flow_area_m2,
            },
        },
        .next = {2, 6}
    },
    [2] = {
        .type = g_is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = g_engine_2_cyl_channel_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_2_cyl_intake_channel_max_flow_area_m2,
            },
            .valve = {
                .engage_r = g_engine_2_cyl_0_theta_r + g_engine_2_cyl_intake_stroke_theta_r,
                .ramp_r = g_engine_2_cyl_valve_ramp_r,
            },
        },
        .next = {4}
    },
    [3] = {
        .type = g_is_injector,
        .as.injector = {
            .chamber = {
                .volume_m3 = g_engine_2_cyl_channel_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_2_cyl_injector_max_flow_area_m2,
            },
            .nozzle_index = 2,
        },
        .next = {4}
    },
    [4] = {
        .type = g_is_piston,
        .as.piston = {
            .chamber = {
                .nozzle_max_flow_area_m2 = g_engine_2_cyl_exhaust_channel_max_flow_area_m2,
            },
            .valve = {
                .engage_r = g_engine_2_cyl_0_theta_r + g_engine_2_cyl_exhaust_stroke_theta_r,
                .ramp_r = g_engine_2_cyl_valve_ramp_r,
            },
            .sparkplug = {
                .engage_r = g_engine_2_cyl_0_theta_r + g_engine_2_cyl_power_stroke_theta_r,
                .on_r = g_engine_2_cyl_sparkplug_on_theta_r,
            },
            .diameter_m = g_engine_2_cyl_piston_diameter_m,
            .theta_r = g_engine_2_cyl_0_theta_r,
            .crank_throw_length_m = g_engine_2_cyl_piston_crank_throw_length_m,
            .connecting_rod_length_m = g_engine_2_cyl_piston_connecting_rod_length_m,
            .connecting_rod_mass_kg = g_engine_2_cyl_piston_connecting_rod_mass_kg,
            .head_mass_density_kg_per_m3 = g_engine_2_cyl_piston_head_mass_density_kg_per_m3,
            .head_compression_height_m = g_engine_2_cyl_piston_head_compression_height_m,
            .head_clearance_height_m = g_engine_2_cyl_piston_head_clearance_height_m,
            .dynamic_friction_coefficient_n_m_s_per_r = g_engine_2_cyl_piston_dynamic_friction_coefficient_n_m_s_per_r,
            .static_friction_coefficient_n_m_s_per_r = g_engine_2_cyl_piston_static_friction_coefficient_n_m_s_per_r,
        },
        .next = {5}
    },
    [5] = {
        .type = g_is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = g_engine_2_cyl_channel_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_2_cyl_exhaust_channel_max_flow_area_m2,
            },
        },
        .next = {10}
    },
    [6] = {
        .type = g_is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = g_engine_2_cyl_channel_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_2_cyl_intake_channel_max_flow_area_m2,
            },
            .valve = {
                .engage_r = g_engine_2_cyl_1_theta_r + g_engine_2_cyl_intake_stroke_theta_r,
                .ramp_r = g_engine_2_cyl_valve_ramp_r,
            },
        },
        .next = {8}
    },
    [7] = {
        .type = g_is_injector,
        .as.injector = {
            .chamber = {
                .volume_m3 = g_engine_2_cyl_channel_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_2_cyl_injector_max_flow_area_m2,
            },
            .nozzle_index = 6,
        },
        .next = {8}
    },
    [8] = {
        .type = g_is_piston,
        .as.piston = {
            .chamber = {
                .nozzle_max_flow_area_m2 = g_engine_2_cyl_exhaust_channel_max_flow_area_m2,
            },
            .valve = {
                .engage_r = g_engine_2_cyl_1_theta_r + g_engine_2_cyl_exhaust_stroke_theta_r,
                .ramp_r = g_engine_2_cyl_valve_ramp_r,
            },
            .sparkplug = {
                .engage_r = g_engine_2_cyl_1_theta_r + g_engine_2_cyl_power_stroke_theta_r,
                .on_r = g_engine_2_cyl_sparkplug_on_theta_r,
            },
            .diameter_m = g_engine_2_cyl_piston_diameter_m,
            .theta_r = g_engine_2_cyl_1_theta_r,
            .crank_throw_length_m = g_engine_2_cyl_piston_crank_throw_length_m,
            .connecting_rod_length_m = g_engine_2_cyl_piston_connecting_rod_length_m,
            .connecting_rod_mass_kg = g_engine_2_cyl_piston_connecting_rod_mass_kg,
            .head_mass_density_kg_per_m3 = g_engine_2_cyl_piston_head_mass_density_kg_per_m3,
            .head_compression_height_m = g_engine_2_cyl_piston_head_compression_height_m,
            .head_clearance_height_m = g_engine_2_cyl_piston_head_clearance_height_m,
            .dynamic_friction_coefficient_n_m_s_per_r = g_engine_2_cyl_piston_dynamic_friction_coefficient_n_m_s_per_r,
            .static_friction_coefficient_n_m_s_per_r = g_engine_2_cyl_piston_static_friction_coefficient_n_m_s_per_r,
        },
        .next = {9}
    },
    [9] = {
        .type = g_is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = g_engine_2_cyl_channel_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_2_cyl_exhaust_channel_max_flow_area_m2,
            },
        },
        .next = {10}
    },
    [10] = {
        .type = g_is_eplenum,
        .as.eplenum = {
            .chamber = {
                .volume_m3 = g_engine_2_cyl_junction_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_2_cyl_exhaust_junction_max_flow_area_m2,
            },
            .wave_index = 0,
        },
        .next = {11}
    },
    [11] = {
        .type = g_is_exhaust,
        .as.exhaust = {
            .chamber = {
                .volume_m3 = g_engine_2_cyl_junction_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_2_cyl_exhaust_junction_max_flow_area_m2,
            },
        },
        .next = {12}
    },
    [12] = {
        .type = g_is_sink,
        .as.sink = {
            .chamber = {
                .volume_m3 = g_chamber_ambient_volume_m3,
            },
        },
        .next = {}
    },
};

static struct engine_s g_engine_2_cyl = {
    "twin",
    engine_is(g_node_2_cyl),
    .crankshaft = {
        .mass_kg = 1.0,
        .radius_m = 0.04,
    },
    .flywheel = {
        .mass_kg = 2.3,
        .radius_m = 0.07,
    },
    .limiter = {
        .cutoff_angular_velocity_r_per_s = 1100.0,
        .relaxed_angular_velocity_r_per_s = 200.0,
    },
    .starter = {
        .rated_torque_n_m = 20.0,
        .no_load_angular_velocity_r_per_s = 500.0,
        .radius_m = 0.015,
    },
};
