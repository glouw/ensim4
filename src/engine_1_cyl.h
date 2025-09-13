static constexpr double g_engine_1_cyl_intake_channel_max_flow_area_m2 = 2.5e-3;
static constexpr double g_engine_1_cyl_exhaust_channel_max_flow_area_m2 = 3.5e-3;
static constexpr double g_engine_1_cyl_throttle_max_flow_area_m2 = g_engine_1_cyl_intake_channel_max_flow_area_m2 / 32.0;
static constexpr double g_engine_1_cyl_injector_max_flow_area_m2 = g_engine_1_cyl_intake_channel_max_flow_area_m2 / 64.0;
static constexpr double g_engine_1_cyl_sparkplug_on_theta_r = (2.0 / 32.0) * g_std_four_pi_r;
static constexpr double g_engine_1_cyl_channel_volume_m3 = 5.0e-4;
static constexpr double g_engine_1_cyl_intake_valve_ramp_r = (0.75 / 4.0) * g_std_four_pi_r;
static constexpr double g_engine_1_cyl_exhaust_valve_ramp_r = (0.5 / 4.0) * g_std_four_pi_r;
static constexpr double g_engine_1_cyl_intake_stroke_theta_r = g_engine_intake_stroke_theta_r + (0.0 / 4.0) * g_std_four_pi_r;
static constexpr double g_engine_1_cyl_exhaust_stroke_theta_r = g_engine_exhaust_stroke_theta_r + (0.2 / 4.0) * g_std_four_pi_r;
static constexpr double g_engine_1_cyl_power_stroke_theta_r = g_engine_power_stroke_theta_r + (0.15 / 4.0) * g_std_four_pi_r;
static constexpr double g_engine_1_cyl_piston_dynamic_friction_coefficient_n_m_s_per_r = 0.02;
static constexpr double g_engine_1_cyl_piston_static_friction_coefficient_n_m_s_per_r = 0.9;
static constexpr double g_engine_1_cyl_piston_diameter_m = 0.096;
static constexpr double g_engine_1_cyl_piston_crank_throw_length_m = 0.03105;
static constexpr double g_engine_1_cyl_piston_connecting_rod_length_m = 0.1055;
static constexpr double g_engine_1_cyl_piston_connecting_rod_mass_kg = 0.4;
static constexpr double g_engine_1_cyl_piston_head_mass_density_kg_per_m3 = 7800.0;
static constexpr double g_engine_1_cyl_piston_head_compression_height_m = 0.0143;
static constexpr double g_engine_1_cyl_piston_head_clearance_height_m = 0.007;
static constexpr double g_engine_1_cyl_gas_momentum_damping_time_constant_s = 1.5e-3;
static constexpr double g_engine_1_cyl_eplenum_pipe_length_m = 0.6;

static struct node_s g_node_1_cyl[] = {
    [0] = {
        .type = g_is_source,
        .as.source = {
            .chamber = {
                .volume_m3 = g_chamber_ambient_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_1_cyl_intake_channel_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_1_cyl_gas_momentum_damping_time_constant_s,
            },
        },
        .next = {1}
    },
    [1] = {
        .type = g_is_throttle,
        .as.throttle = {
            .chamber = {
                .volume_m3 = g_engine_1_cyl_channel_volume_m3,
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
                .volume_m3 = g_engine_1_cyl_channel_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_1_cyl_intake_channel_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_1_cyl_gas_momentum_damping_time_constant_s,
            },
            .valve = {
                .engage_r = g_engine_1_cyl_intake_stroke_theta_r,
                .ramp_r = g_engine_1_cyl_intake_valve_ramp_r,
            },
        },
        .next = {4}
    },
    [3] = {
        .type = g_is_injector,
        .as.injector = {
            .chamber = {
                .volume_m3 = g_engine_1_cyl_channel_volume_m3,
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
                .nozzle_max_flow_area_m2 = g_engine_1_cyl_exhaust_channel_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_1_cyl_gas_momentum_damping_time_constant_s,
            },
            .valve = {
                .engage_r = g_engine_1_cyl_exhaust_stroke_theta_r,
                .ramp_r = g_engine_1_cyl_exhaust_valve_ramp_r,
            },
            .sparkplug = {
                .engage_r = g_engine_1_cyl_power_stroke_theta_r,
                .on_r = g_engine_1_cyl_sparkplug_on_theta_r,
            },
            .diameter_m = g_engine_1_cyl_piston_diameter_m,
            .crank_throw_length_m = g_engine_1_cyl_piston_crank_throw_length_m,
            .connecting_rod_length_m = g_engine_1_cyl_piston_connecting_rod_length_m,
            .connecting_rod_mass_kg = g_engine_1_cyl_piston_connecting_rod_mass_kg,
            .head_mass_density_kg_per_m3 = g_engine_1_cyl_piston_head_mass_density_kg_per_m3,
            .head_compression_height_m = g_engine_1_cyl_piston_head_compression_height_m,
            .head_clearance_height_m = g_engine_1_cyl_piston_head_clearance_height_m,
            .dynamic_friction_coefficient_n_m_s_per_r = g_engine_1_cyl_piston_dynamic_friction_coefficient_n_m_s_per_r,
            .static_friction_coefficient_n_m_s_per_r = g_engine_1_cyl_piston_static_friction_coefficient_n_m_s_per_r,
        },
        .next = {5}
    },
    [5] = {
        .type = g_is_erunner,
        .as.erunner = {
            .chamber = {
                .volume_m3 = 0.5 * g_engine_1_cyl_channel_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_1_cyl_exhaust_channel_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_1_cyl_gas_momentum_damping_time_constant_s,
            },
        },
        .next = {6}
    },
    [6] = {
        .type = g_is_eplenum,
        .as.eplenum = {
            .chamber = {
                .volume_m3 = 4.0 * g_engine_1_cyl_channel_volume_m3,
                .nozzle_max_flow_area_m2 = g_engine_1_cyl_exhaust_channel_max_flow_area_m2,
                .gas_momentum_damping_time_constant_s = g_engine_1_cyl_gas_momentum_damping_time_constant_s,
            },
            .wave_index = 0,
            .pipe_length_m = g_engine_1_cyl_eplenum_pipe_length_m,
        },
        .next = {7}
    },
    [7] = {
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
    "trx450",
    engine_is(g_node_1_cyl),
    .crankshaft = {
        .mass_kg = 1.5,
        .radius_m = 0.031,
    },
    .flywheel = {
        .mass_kg = 2.7,
        .radius_m = 0.075,
    },
    .limiter = {
        .cutoff_angular_velocity_r_per_s = 1000.0,
        .relaxed_angular_velocity_r_per_s = 200.0,
    },
    .starter = {
        .rated_torque_n_m = 30.0,
        .no_load_angular_velocity_r_per_s = 500.0,
        .radius_m = 0.015,
    },
    .volume = 0.1,
    .no_throttle = 0.0025,
    .low_throttle = 0.20,
    .mid_throttle = 0.50,
    .high_throttle = 1.00,
    .radial_spacing = 4.0,
};

