constexpr char g_engine_name[] = "Inline 8";
constexpr double g_engine_sound_volume = 0.5;
constexpr double g_engine_radial_spacing = 2.1;
constexpr double g_engine_source_sink_volume_m3 = 1.00e20;
constexpr double g_engine_piston_diameter_m = 0.065;
constexpr double g_engine_piston_crank_throw_length_m = 0.038;
constexpr double g_engine_piston_connecting_rod_length_m = 0.10;
constexpr double g_engine_piston_connecting_rod_mass_kg = 0.40;
constexpr double g_engine_piston_head_mass_density_kg_per_m3 = 7800.0;
constexpr double g_engine_piston_head_compression_height_m = 0.018;
constexpr double g_engine_piston_head_clearance_height_m = 0.007;
constexpr double g_engine_piston_dynamic_friction_n_m_s_per_r = 0.03;
constexpr double g_engine_piston_static_friction_n_m_s_per_r = 0.90;
constexpr double g_engine_gas_momentum_damping_time_constant_s = 0.53e-3;
constexpr double g_engine_eplenum_wave_pipe_length_m = 0.8;
constexpr double g_engine_mic_position_ratio = 0.05;
constexpr double g_engine_velocity_low_pass_cutoff_frequency_hz = 8000.0;
constexpr double g_engine_chamber_volume_m3 = 2.1e-4;
constexpr double g_engine_throttle_volume_m3 = 1.00 * g_engine_chamber_volume_m3;
constexpr double g_engine_irunner_volume_m3 = 1.50 * g_engine_chamber_volume_m3;
constexpr double g_engine_injector_volume_m3 = 0.02 * g_engine_chamber_volume_m3;
constexpr double g_engine_erunner_volume_m3 = 0.40 * g_engine_chamber_volume_m3;
constexpr double g_engine_eplenum_volume_m3 = 0.75 * g_engine_chamber_volume_m3;
constexpr double g_engine_exhaust_volume_m3 = 0.75 * g_engine_chamber_volume_m3;
constexpr double g_engine_max_flow_area_m2 = 2.80e-3;
constexpr double g_engine_source_max_flow_area_m2 = 1.300 * g_engine_max_flow_area_m2;
constexpr double g_engine_throttle_max_flow_area_m2 = 1.250 * g_engine_max_flow_area_m2;
constexpr double g_engine_irunner_max_flow_area_m2 = 0.600 * g_engine_max_flow_area_m2;
constexpr double g_engine_injector_max_flow_area_m2 = 0.005 * g_engine_max_flow_area_m2;
constexpr double g_engine_piston_max_flow_area_m2 = 0.900 * g_engine_max_flow_area_m2;
constexpr double g_engine_erunner_max_flow_area_m2 = 0.450 * g_engine_max_flow_area_m2;
constexpr double g_engine_eplenum_max_flow_area_m2 = 1.800 * g_engine_max_flow_area_m2;
constexpr double g_engine_exhaust_max_flow_area_m2 = 0.900 * g_engine_max_flow_area_m2;
constexpr double g_engine_piston_0_theta_r = (0.00 / 8.0) * 4.0 * g_std_pi_r;
constexpr double g_engine_piston_1_theta_r = (1.00 / 8.0) * 4.0 * g_std_pi_r;
constexpr double g_engine_piston_2_theta_r = (2.00 / 8.0) * 4.0 * g_std_pi_r;
constexpr double g_engine_piston_3_theta_r = (3.00 / 8.0) * 4.0 * g_std_pi_r;
constexpr double g_engine_piston_4_theta_r = (4.00 / 8.0) * 4.0 * g_std_pi_r;
constexpr double g_engine_piston_5_theta_r = (5.00 / 8.0) * 4.0 * g_std_pi_r;
constexpr double g_engine_piston_6_theta_r = (6.00 / 8.0) * 4.0 * g_std_pi_r;
constexpr double g_engine_piston_7_theta_r = (7.00 / 8.0) * 4.0 * g_std_pi_r;
constexpr double g_engine_irunner_valve_engage_r = -0.25 * g_std_pi_r;
constexpr double g_engine_irunner_valve_ramp_r = 1.00 * g_std_pi_r;
constexpr double g_engine_piston_valve_engage_r = 2.70 * g_std_pi_r;
constexpr double g_engine_piston_valve_ramp_r = 0.95 * g_std_pi_r;
constexpr double g_engine_sparkplug_engage_r = 2.05 * g_std_pi_r;
constexpr double g_engine_sparkplug_on_r = 0.25 * g_std_pi_r;
constexpr double g_engine_no_throttle = 0.000;
constexpr double g_engine_low_throttle = 0.001;
constexpr double g_engine_mid_throttle = 0.050;
constexpr double g_engine_high_throttle = 1.0;
constexpr double g_engine_crankshaft_mass_kg = 25.3;
constexpr double g_engine_crankshaft_radius_m = 0.031;
constexpr double g_engine_flywheel_mass_kg = 8.15;
constexpr double g_engine_flywheel_radius_m = 0.18;
constexpr double g_engine_limiter_cutoff_r_per_s = 1700.0;
constexpr double g_engine_limiter_relaxed_r_per_s = 50.0;
constexpr double e_engine_starter_rated_torque_n_m = 70.0;
constexpr double g_engine_starter_no_load_r_per_s = 700.0;
constexpr double g_engine_starter_radius_m = 0.015;

static struct node_s g_engine_node[] = {

    [0] = source_d({1}),
    [1] = throttle_d({2, 6, 10, 14, 18, 22, 26, 30}),

    [2] = irunner_d(g_engine_piston_0_theta_r, {4}),
    [3] = injector_d(2, {4}),
    [4] = piston_d(g_engine_piston_0_theta_r, {5}),
    [5] = erunner_d({34}),

    [6] = irunner_d(g_engine_piston_1_theta_r, {8}),
    [7] = injector_d(6, {8}),
    [8] = piston_d(g_engine_piston_1_theta_r, {9}),
    [9] = erunner_d({36}),

    [10] = irunner_d(g_engine_piston_2_theta_r, {12}),
    [11] = injector_d(10, {12}),
    [12] = piston_d(g_engine_piston_2_theta_r, {13}),
    [13] = erunner_d({34}),

    [14] = irunner_d(g_engine_piston_3_theta_r, {16}),
    [15] = injector_d(14, {16}),
    [16] = piston_d(g_engine_piston_3_theta_r, {17}),
    [17] = erunner_d({36}),

    [18] = irunner_d(g_engine_piston_4_theta_r, {20}),
    [19] = injector_d(18, {20}),
    [20] = piston_d(g_engine_piston_4_theta_r, {21}),
    [21] = erunner_d({34}),

    [22] = irunner_d(g_engine_piston_5_theta_r, {24}),
    [23] = injector_d(22, {24}),
    [24] = piston_d(g_engine_piston_5_theta_r, {25}),
    [25] = erunner_d({36}),

    [26] = irunner_d(g_engine_piston_6_theta_r, {28}),
    [27] = injector_d(26, {28}),
    [28] = piston_d(g_engine_piston_6_theta_r, {29}),
    [29] = erunner_d({34}),

    [30] = irunner_d(g_engine_piston_7_theta_r, {32}),
    [31] = injector_d(30, {32}),
    [32] = piston_d(g_engine_piston_7_theta_r, {33}),
    [33] = erunner_d({36}),

    [34] = eplenum_d(0, {35}),
    [35] = exhaust_d({38}),

    [36] = eplenum_d(1, {37}),
    [37] = exhaust_d({38}),

    [38] = sink_d(),
};
