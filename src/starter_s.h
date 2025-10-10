struct starter_s
{
    double rated_torque_n_m;
    double no_load_angular_velocity_r_per_s;
    double radius_m;
    bool is_on;
};

static double
calc_flywheel_to_starter_gear_ratio(
    struct starter_s* self,
    struct flywheel_s* flywheel)
{
    double starter_gear_ratio = flywheel->radius_m / self->radius_m;
    return starter_gear_ratio;
}

static double
calc_starter_angular_velocity_r_per_s(
    struct starter_s* self,
    struct flywheel_s* flywheel,
    struct crankshaft_s* crankshaft)
{
    if(self->is_on == false)
    {
        return 0.0;
    }
    double gear_ratio = calc_flywheel_to_starter_gear_ratio(self, flywheel);
    double starter_angular_velocity_r_per_s = max(crankshaft->angular_velocity_r_per_s * gear_ratio, 0.0);
    return starter_angular_velocity_r_per_s;
}

static double
calc_starter_torque_on_flywheel_n_m(
    struct starter_s* self,
    struct flywheel_s* flywheel,
    struct crankshaft_s* crankshaft)
{
    if(self->is_on == false)
    {
        return 0.0;
    }
    double starter_angular_velocity_r_per_s = calc_starter_angular_velocity_r_per_s(self, flywheel, crankshaft);
    if(starter_angular_velocity_r_per_s >= self->no_load_angular_velocity_r_per_s)
    {
        return 0.0;
    }
    double angular_velocity_ratio = starter_angular_velocity_r_per_s / self->no_load_angular_velocity_r_per_s;
    double starter_torque_n_m = self->rated_torque_n_m * (1.0 - angular_velocity_ratio);
    double gear_ratio = calc_flywheel_to_starter_gear_ratio(self, flywheel);
    double flywheel_torque_n_m = starter_torque_n_m * gear_ratio;
    return flywheel_torque_n_m;
}
