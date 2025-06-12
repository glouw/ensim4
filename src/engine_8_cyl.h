static struct node_s g_node_8_cyl[] = {
    [0] = {
        .type = is_source,
        .as.source = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001,
            },
        },
        .next = {1}
    },
    [1] = {
        .type = is_filter,
        .as.filter = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {2}
    },
    [2] = {
        .type = is_throttle,
        .as.throttle = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {3}
    },
    [3] = {
        .type = is_iplenum,
        .as.iplenum = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {4, 5, 6, 7, 8, 9, 10, 11}
    },
    [4] = {
        .type = is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {12}
    },
    [5] = {
        .type = is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {13}
    },
    [6] = {
        .type = is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {14}
    },
    [7] = {
        .type = is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {15}
    },
    [8] = {
        .type = is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {16}
    },
    [9] = {
        .type = is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {17}
    },
    [10] = {
        .type = is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {18}
    },
    [11] = {
        .type = is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {19}
    },
    [12] = {
        .type = is_piston,
        .as.piston = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
            .diameter_m = 0.08,
            .theta_r = (0.0 / 8.0) * g_std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.01,
            .head_clearance_height_m = 0.001,
            .friction_coefficient_n_m_s_per_r = 0.01,
        },
        .next = {20}
    },
    [13] = {
        .type = is_piston,
        .as.piston = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
            .diameter_m = 0.08,
            .theta_r = (1.0 / 8.0) * g_std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.01,
            .head_clearance_height_m = 0.001,
            .friction_coefficient_n_m_s_per_r = 0.01,
        },
        .next = {21}
    },
    [14] = {
        .type = is_piston,
        .as.piston = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
            .diameter_m = 0.08,
            .theta_r = (2.0 / 8.0) * g_std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.01,
            .head_clearance_height_m = 0.001,
            .friction_coefficient_n_m_s_per_r = 0.01,
        },
        .next = {22}
    },
    [15] = {
        .type = is_piston,
        .as.piston = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
            .diameter_m = 0.08,
            .theta_r = (3.0 / 8.0) * g_std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.01,
            .head_clearance_height_m = 0.001,
            .friction_coefficient_n_m_s_per_r = 0.01,
        },
        .next = {23}
    },
    [16] = {
        .type = is_piston,
        .as.piston = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
            .diameter_m = 0.08,
            .theta_r = (4.0 / 8.0) * g_std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.01,
            .head_clearance_height_m = 0.001,
            .friction_coefficient_n_m_s_per_r = 0.01,
        },
        .next = {24}
    },
    [17] = {
        .type = is_piston,
        .as.piston = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
            .diameter_m = 0.08,
            .theta_r = (5.0 / 8.0) * g_std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.01,
            .head_clearance_height_m = 0.001,
            .friction_coefficient_n_m_s_per_r = 0.01,
        },
        .next = {25}
    },
    [18] = {
        .type = is_piston,
        .as.piston = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
            .diameter_m = 0.08,
            .theta_r = (6.0 / 8.0) * g_std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.01,
            .head_clearance_height_m = 0.001,
            .friction_coefficient_n_m_s_per_r = 0.01,
        },
        .next = {26}
    },
    [19] = {
        .type = is_piston,
        .as.piston = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
            .diameter_m = 0.08,
            .theta_r = (7.0 / 8.0) * g_std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.01,
            .head_clearance_height_m = 0.001,
            .friction_coefficient_n_m_s_per_r = 0.01,
        },
        .next = {27}
    },
    [20] = {
        .type = is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {28}
    },
    [21] = {
        .type = is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {28}
    },
    [22] = {
        .type = is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {28}
    },
    [23] = {
        .type = is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {28}
    },
    [24] = {
        .type = is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {28}
    },
    [25] = {
        .type = is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {28}
    },
    [26] = {
        .type = is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {28}
    },
    [27] = {
        .type = is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {28}
    },
    [28] = {
        .type = is_eplenum,
        .as.eplenum = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {29}
    },
    [29] = {
        .type = is_exhaust,
        .as.eplenum = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {30}
    },
    [30] = {
        .type = is_sink,
        .as.sink = {
            .chamber = {
                .volume_m3 = 1.0,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.0001
            },
        },
        .next = {}
    },
};

static struct engine_s g_engine_8_cyl = {
    .node = g_node_8_cyl,
    .size = len(g_node_8_cyl),
    .crankshaft = {
        .mass_kg = 0.25,
        .radius_m = 0.01,
    },
    .flywheel = {
        .mass_kg = 1.0,
        .radius_m = 0.1,
    },
    .starter = {
        .rated_torque_n_m = 30.0,
        .no_load_speed_r_per_s = 1575.0,
        .radius_m = 0.008,
    },
};
