static struct node_s g_node_8_cyl[] = {
    [0] = {
        .type = g_is_source,
        .as.source = {
            .chamber = {
                .volume_m3 = 1e6,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.006
            },
        },
        .next = {1}
    },
    [1] = {
        .type = g_is_filter,
        .as.filter = {
            .chamber = {
                .volume_m3 = 0.0015,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.006
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
                .nozzle_max_flow_area_m2 = 0.006
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
                .nozzle_max_flow_area_m2 = 0.006
            },
        },
        .next = {4, 5, 6, 7, 8, 9, 10, 11}
    },
    [4] = {
        .type = g_is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 0.0003,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.00075
            },
            .valve = {
                .engage_r = (-1.0 / 8.0) * g_std_four_pi_r,
                .ramp_r = g_std_pi_r,
            },
        },
        .next = {12}
    },
    [5] = {
        .type = g_is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 0.0003,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.00075
            },
            .valve = {
                .engage_r = (0.0 / 8.0) * g_std_four_pi_r,
                .ramp_r = g_std_pi_r,
            },
        },
        .next = {13}
    },
    [6] = {
        .type = g_is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 0.0003,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.00075
            },
            .valve = {
                .engage_r = (1.0 / 8.0) * g_std_four_pi_r,
                .ramp_r = g_std_pi_r,
            },
        },
        .next = {14}
    },
    [7] = {
        .type = g_is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 0.0003,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.00075
            },
            .valve = {
                .engage_r = (2.0 / 8.0) * g_std_four_pi_r,
                .ramp_r = g_std_pi_r,
            },
        },
        .next = {15}
    },
    [8] = {
        .type = g_is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 0.0003,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.00075
            },
            .valve = {
                .engage_r = (3.0 / 8.0) * g_std_four_pi_r,
                .ramp_r = g_std_pi_r,
            },
        },
        .next = {16}
    },
    [9] = {
        .type = g_is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 0.0003,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.00075
            },
            .valve = {
                .engage_r = (4.0 / 8.0) * g_std_four_pi_r,
                .ramp_r = g_std_pi_r,
            },
        },
        .next = {17}
    },
    [10] = {
        .type = g_is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 0.0003,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.00075
            },
            .valve = {
                .engage_r = (5.0 / 8.0) * g_std_four_pi_r,
                .ramp_r = g_std_pi_r,
            },
        },
        .next = {18}
    },
    [11] = {
        .type = g_is_irunner,
        .as.irunner = {
            .chamber = {
                .volume_m3 = 0.0003,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.00075
            },
            .valve = {
                .engage_r = (6.0 / 8.0) * g_std_four_pi_r,
                .ramp_r = g_std_pi_r,
            },
        },
        .next = {19}
    },
    [12] = {
        .type = g_is_piston,
        .as.piston = {
            .chamber = {
                .nozzle_max_flow_area_m2 = 0.00075
            },
            .valve = {
                .engage_r = (5.0 / 8.0) * g_std_four_pi_r,
                .ramp_r = g_std_pi_r,
            },
            .diameter_m = 0.08,
            .theta_r = (0.0 / 8.0) * g_std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.025,
            .head_clearance_height_m = 0.01,
            .dynamic_friction_coefficient_n_m_s_per_r = 0.13,
            .static_friction_coefficient_n_m_s_per_r = 4.0,
        },
        .next = {20}
    },
    [13] = {
        .type = g_is_piston,
        .as.piston = {
            .chamber = {
                .nozzle_max_flow_area_m2 = 0.00075
            },
            .valve = {
                .engage_r = (6.0 / 8.0) * g_std_four_pi_r,
                .ramp_r = g_std_pi_r,
            },
            .diameter_m = 0.08,
            .theta_r = (1.0 / 8.0) * g_std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.025,
            .head_clearance_height_m = 0.01,
            .dynamic_friction_coefficient_n_m_s_per_r = 0.13,
            .static_friction_coefficient_n_m_s_per_r = 4.0,
        },
        .next = {21}
    },
    [14] = {
        .type = g_is_piston,
        .as.piston = {
            .chamber = {
                .nozzle_max_flow_area_m2 = 0.00075
            },
            .valve = {
                .engage_r = (7.0 / 8.0) * g_std_four_pi_r,
                .ramp_r = g_std_pi_r,
            },
            .diameter_m = 0.08,
            .theta_r = (2.0 / 8.0) * g_std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.025,
            .head_clearance_height_m = 0.01,
            .dynamic_friction_coefficient_n_m_s_per_r = 0.13,
            .static_friction_coefficient_n_m_s_per_r = 4.0,
        },
        .next = {22}
    },
    [15] = {
        .type = g_is_piston,
        .as.piston = {
            .chamber = {
                .nozzle_max_flow_area_m2 = 0.00075
            },
            .valve = {
                .engage_r = (0.0 / 8.0) * g_std_four_pi_r,
                .ramp_r = g_std_pi_r,
            },
            .diameter_m = 0.08,
            .theta_r = (3.0 / 8.0) * g_std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.025,
            .head_clearance_height_m = 0.01,
            .dynamic_friction_coefficient_n_m_s_per_r = 0.13,
            .static_friction_coefficient_n_m_s_per_r = 4.0,
        },
        .next = {23}
    },
    [16] = {
        .type = g_is_piston,
        .as.piston = {
            .chamber = {
                .nozzle_max_flow_area_m2 = 0.00075
            },
            .valve = {
                .engage_r = (1.0 / 8.0) * g_std_four_pi_r,
                .ramp_r = g_std_pi_r,
            },
            .diameter_m = 0.08,
            .theta_r = (4.0 / 8.0) * g_std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.025,
            .head_clearance_height_m = 0.01,
            .dynamic_friction_coefficient_n_m_s_per_r = 0.13,
            .static_friction_coefficient_n_m_s_per_r = 4.0,
        },
        .next = {24}
    },
    [17] = {
        .type = g_is_piston,
        .as.piston = {
            .chamber = {
                .nozzle_max_flow_area_m2 = 0.00075
            },
            .valve = {
                .engage_r = (2.0 / 8.0) * g_std_four_pi_r,
                .ramp_r = g_std_pi_r,
            },
            .diameter_m = 0.08,
            .theta_r = (5.0 / 8.0) * g_std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.025,
            .head_clearance_height_m = 0.01,
            .dynamic_friction_coefficient_n_m_s_per_r = 0.13,
            .static_friction_coefficient_n_m_s_per_r = 4.0,
        },
        .next = {25}
    },
    [18] = {
        .type = g_is_piston,
        .as.piston = {
            .chamber = {
                .nozzle_max_flow_area_m2 = 0.00075
            },
            .valve = {
                .engage_r = (3.0 / 8.0) * g_std_four_pi_r,
                .ramp_r = g_std_pi_r,
            },
            .diameter_m = 0.08,
            .theta_r = (6.0 / 8.0) * g_std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.025,
            .head_clearance_height_m = 0.01,
            .dynamic_friction_coefficient_n_m_s_per_r = 0.13,
            .static_friction_coefficient_n_m_s_per_r = 4.0,
        },
        .next = {26}
    },
    [19] = {
        .type = g_is_piston,
        .as.piston = {
            .chamber = {
                .nozzle_max_flow_area_m2 = 0.00075
            },
            .valve = {
                .engage_r = (4.0 / 8.0) * g_std_four_pi_r,
                .ramp_r = g_std_pi_r,
            },
            .diameter_m = 0.08,
            .theta_r = (7.0 / 8.0) * g_std_four_pi_r,
            .crank_throw_length_m = 0.04,
            .connecting_rod_length_m = 0.12,
            .connecting_rod_mass_kg = 0.5,
            .head_mass_density_kg_per_m3 = 7800.0,
            .head_compression_height_m = 0.025,
            .head_clearance_height_m = 0.01,
            .dynamic_friction_coefficient_n_m_s_per_r = 0.13,
            .static_friction_coefficient_n_m_s_per_r = 4.0,
        },
        .next = {27}
    },
    [20] = {
        .type = g_is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 0.0003,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.00075
            },
        },
        .next = {28}
    },
    [21] = {
        .type = g_is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 0.0003,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.00075
            },
        },
        .next = {28}
    },
    [22] = {
        .type = g_is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 0.0003,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.00075
            },
        },
        .next = {29}
    },
    [23] = {
        .type = g_is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 0.0003,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.00075
            },
        },
        .next = {29}
    },
    [24] = {
        .type = g_is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 0.0003,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.00075
            },
        },
        .next = {30}
    },
    [25] = {
        .type = g_is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 0.0003,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.00075
            },
        },
        .next = {30}
    },
    [26] = {
        .type = g_is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 0.0003,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.00075
            },
        },
        .next = {31}
    },
    [27] = {
        .type = g_is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 0.0003,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.00075
            },
        },
        .next = {31}
    },
    [28] = {
        .type = g_is_eplenum,
        .as.eplenum = {
            .chamber = {
                .volume_m3 = 0.15,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.006
            },
            .wave_index = 0,
        },
        .next = {32}
    },
    [29] = {
        .type = g_is_eplenum,
        .as.eplenum = {
            .chamber = {
                .volume_m3 = 0.15,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.006
            },
            .wave_index = 1,
        },
        .next = {33}
    },
    [30] = {
        .type = g_is_eplenum,
        .as.eplenum = {
            .chamber = {
                .volume_m3 = 0.15,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.006
            },
            .wave_index = 2,
        },
        .next = {34}
    },
    [31] = {
        .type = g_is_eplenum,
        .as.eplenum = {
            .chamber = {
                .volume_m3 = 0.15,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.006
            },
            .wave_index = 3,
        },
        .next = {35}
    },
    [32] = {
        .type = g_is_exhaust,
        .as.exhaust = {
            .chamber = {
                .volume_m3 = 0.1,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.006
            },
        },
        .next = {36}
    },
    [33] = {
        .type = g_is_exhaust,
        .as.exhaust = {
            .chamber = {
                .volume_m3 = 0.1,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.006
            },
        },
        .next = {36}
    },
    [34] = {
        .type = g_is_exhaust,
        .as.exhaust = {
            .chamber = {
                .volume_m3 = 0.1,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.006
            },
        },
        .next = {36}
    },
    [35] = {
        .type = g_is_exhaust,
        .as.exhaust = {
            .chamber = {
                .volume_m3 = 0.1,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.006
            },
        },
        .next = {36}
    },
    [36] = {
        .type = g_is_sink,
        .as.sink = {
            .chamber = {
                .volume_m3 = 1e6,
                .nozzle_open_ratio = 1.0,
                .nozzle_max_flow_area_m2 = 0.006
            },
        },
        .next = {}
    },
};

static struct engine_s g_engine_8_cyl = {
    "inline 8 - quad exhaust",
    engine_is(g_node_8_cyl),
    .crankshaft = {
        .mass_kg = 18.0,
        .radius_m = 0.04,
    },
    .flywheel = {
        .mass_kg = 15.0,
        .radius_m = 0.18,
    },
    .starter = {
        .rated_torque_n_m = 80.0,
        .no_load_angular_velocity_r_per_s = 500.0,
        .radius_m = 0.015,
    },
};
