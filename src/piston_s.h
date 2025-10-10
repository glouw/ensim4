/* ------- + block_deck_surface_m
 *         | head_clearance_height_m
 * ------- +
 * |     | | head_compression_height_m
 * |  o  | + pin_x_m, pin_y_m
 * |     | |
 * |-----| |
 *   | |   |
 *   | |   | connecting_rod_length_m
 *   | |   |
 *   | |   |
 *   | |   |
 *   |o|   + bearing_x_m, bearing_y_m
 *    |    |
 *    |    | crank_throw_length_m
 *    |    |
 *    o    + origin
 */

constexpr double g_static_friction_upper_angular_velocity_r_per_s = g_std_four_pi_r;

struct piston_s
{
    struct chamber_s chamber;
    struct valve_s valve;
    struct sparkplug_s sparkplug;
    double diameter_m;
    double pin_x_m;
    double pin_y_m;
    double bearing_x_m;
    double bearing_y_m;
    double theta_r;
    double crank_throw_length_m;
    double connecting_rod_length_m;
    double connecting_rod_mass_kg;
    double head_mass_density_kg_per_m3;
    double head_compression_height_m;
    double head_clearance_height_m;
    double dynamic_friction_n_m_s_per_r;
    double static_friction_n_m_s_per_r;
};

static double
calc_piston_theta_r(struct piston_s* self, struct crankshaft_s* crankshaft)
{
    return crankshaft->theta_r + self->theta_r;
}

static double
calc_piston_top_dead_center_m(struct piston_s* self)
{
    return self->connecting_rod_length_m + self->crank_throw_length_m + self->head_compression_height_m;
}

static double
calc_piston_block_deck_surface_m(struct piston_s* self)
{
    return calc_piston_top_dead_center_m(self) + self->head_clearance_height_m;
}

static double
calc_piston_chamber_depth_at_m(struct piston_s* self, double y_m)
{
    return calc_piston_block_deck_surface_m(self) - (y_m + self->head_compression_height_m);
}

static double
calc_piston_chamber_depth_m(struct piston_s* self)
{
    return calc_piston_chamber_depth_at_m(self, self->pin_y_m);
}

static double
calc_piston_gas_torque_n_m(struct piston_s* self, struct crankshaft_s* crankshaft)
{
    double theta_r = calc_piston_theta_r(self, crankshaft);
    double term1 = calc_static_gauge_pressure_pa(&self->chamber) * calc_circle_area_m2(self->diameter_m) * self->crank_throw_length_m * sin(theta_r);
    double term2 = 1.0 + (self->crank_throw_length_m / self->connecting_rod_length_m) * cos(theta_r);
    return term1 * term2;
}

static double
calc_piston_head_mass_kg(struct piston_s* self)
{
    return self->head_mass_density_kg_per_m3 * calc_cylinder_volume_m3(self->diameter_m, 2.0 * self->head_compression_height_m);
}

static double
calc_piston_volume_m3(struct piston_s* self)
{
    double depth_m = calc_piston_chamber_depth_m(self);
    return calc_cylinder_volume_m3(self->diameter_m, depth_m);
}

static double
calc_piston_moment_of_inertia_kg_per_m2(struct piston_s* self)
{
    double mass_reciprocating_kg = calc_piston_head_mass_kg(self) + 0.5 * self->connecting_rod_mass_kg;
    return mass_reciprocating_kg * pow(self->crank_throw_length_m, 2.0);
}

static double
calc_piston_inertia_torque_n_m(struct piston_s* self, struct crankshaft_s* crankshaft)
{
    double theta_r = calc_piston_theta_r(self, crankshaft);
    double term1 = 0.25 * sin(1.0 * theta_r) * self->crank_throw_length_m / self->connecting_rod_length_m;
    double term2 = 0.50 * sin(2.0 * theta_r);
    double term3 = 0.75 * sin(3.0 * theta_r) * self->crank_throw_length_m / self->connecting_rod_length_m;
    return calc_piston_moment_of_inertia_kg_per_m2(self) * pow(crankshaft->angular_velocity_r_per_s, 2.0) * (term1 - term2 - term3);
}

static double
calc_piston_friction_torque_n_m(struct piston_s* self, struct crankshaft_s* crankshaft)
{
    bool is_static = fabs(crankshaft->angular_velocity_r_per_s) < g_static_friction_upper_angular_velocity_r_per_s;
    double friction_n_m_s_per_r = is_static ? self->static_friction_n_m_s_per_r : self->dynamic_friction_n_m_s_per_r;
    double direction = -1.0; // Opposes.
    return direction * crankshaft->angular_velocity_r_per_s * friction_n_m_s_per_r;
}

static void
update_piston_bearing_position(struct piston_s* self, double theta_r)
{
    self->bearing_x_m = self->crank_throw_length_m * sin(theta_r);
    self->bearing_y_m = self->crank_throw_length_m * cos(theta_r);
}

static void
update_piston_pin_position(struct piston_s* self, double theta_r)
{
    self->pin_x_m = 0.0;
    double term1 = sqrt(pow(self->connecting_rod_length_m, 2.0) - pow(self->crank_throw_length_m * sin(theta_r), 2.0));
    double term2 = self->crank_throw_length_m * cos(theta_r);
    self->pin_y_m = term1 + term2;
}

static void
rig_piston(struct piston_s* self, struct crankshaft_s* crankshaft)
{
    double theta_r = calc_piston_theta_r(self, crankshaft);
    update_piston_bearing_position(self, theta_r);
    update_piston_pin_position(self, theta_r);
    self->chamber.volume_m3 = calc_piston_volume_m3(self);
}

static void
compress_piston(struct piston_s* self, struct crankshaft_s* crankshaft)
{
    double old_volume_m3 = calc_piston_volume_m3(self);
    rig_piston(self, crankshaft);
    self->chamber.gas.static_temperature_k = calc_new_adiabatic_static_temperature_from_volume_delta_k(&self->chamber, old_volume_m3);
}
