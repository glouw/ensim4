static struct node_s g_node_1_cyl[] = {
    [0] = {
        .type = g_is_source,
        .as.source = {
            .chamber = {
                .volume_m3 = 1e9,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.001
            },
        },
        .next = {1}
    },
    [1] = {
        .type = g_is_filter,
        .as.filter = {
            .chamber = {
                .volume_m3 = 0.001,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.001
            },
        },
        .next = {2}
    },
    [2] = {
        .type = g_is_throttle,
        .as.throttle = {
            .chamber = {
                .volume_m3 = 0.0005,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.001
            },
        },
        .next = {3}
    },
    [3] = {
        .type = g_is_iplenum,
        .as.iplenum = {
            .chamber = {
                .volume_m3 = 0.002,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.001
            },
        },
        .next = {4}
    },
    [4] = {
        .type = g_is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 0.0002,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.00075
            },
            .valve = {
                .engage_r = (-0.5 / 8.0) * g_std_four_pi_r,
                .ramp_r = (1.0 / 4.0) * g_std_four_pi_r,
            },
        },
        .next = {5}
    },
    [5] = {
        .type = g_is_piston,
        .as.piston = {
            .chamber = {
                .nozzle_max_flow_area_m2 = 0.00075
            },
            .valve = {
                .engage_r = (5.5 / 8.0) * g_std_four_pi_r,
                .ramp_r = (1.0 / 4.0) * g_std_four_pi_r,
            },
            .diameter_m = 0.08,
            .theta_r = g_std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.015,
            .head_clearance_height_m = 0.002,
            .dynamic_friction_coefficient_n_m_s_per_r = 0.13,
            .static_friction_coefficient_n_m_s_per_r = 0.9,
        },
        .next = {6}
    },
    [6] = {
        .type = g_is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 0.0002,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.00075
            },
        },
        .next = {7}
    },
    [7] = {
        .type = g_is_eplenum,
        .as.eplenum = {
            .chamber = {
                .volume_m3 = 0.002,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.00075
            },
        },
        .next = {8}
    },
    [8] = {
        .type = g_is_exhaust,
        .as.eplenum = {
            .chamber = {
                .volume_m3 = 0.01,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.00075
            },
        },
        .next = {9}
    },
    [9] = {
        .type = g_is_sink,
        .as.sink = {
            .chamber = {
                .volume_m3 = 1e9,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.00075
            },
        },
        .next = {}
    },
};

static struct engine_s g_engine_1_cyl = {
    "single piston",
    engine_is(g_node_1_cyl),
    .crankshaft = {
        .mass_kg = 6.0,
        .radius_m = 0.04,
    },
    .flywheel = {
        .mass_kg = 3.0,
        .radius_m = 0.18,
    },
    .starter = {
        .rated_torque_n_m = 40.0,
        .no_load_angular_velocity_r_per_s = 800.0,
        .radius_m = 0.015,
    },
};
