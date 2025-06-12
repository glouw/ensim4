struct starter_s
{
    double rated_torque_n_m;
    double no_load_speed_r_per_s;
    double radius_m;
};

static double
calc_starter_torque_n_m(const struct starter_s* self, const struct flywheel_s* flywheel, const struct crankshaft_s* crankshaft)
{
    double gear_ratio = flywheel->radius_m / self->radius_m;
    double angular_velocity_r_per_s = crankshaft->angular_velocity_r_per_s * gear_ratio;
    angular_velocity_r_per_s = max(angular_velocity_r_per_s, 0.0);
    if(angular_velocity_r_per_s >= self->no_load_speed_r_per_s)
    {
        return 0.0;
    }
    double speed_ratio = angular_velocity_r_per_s / self->no_load_speed_r_per_s;
    double starter_torque_n_m = self->rated_torque_n_m * (1.0 - speed_ratio);
    double flywheel_torque_n_m = starter_torque_n_m * gear_ratio;
    return flywheel_torque_n_m;
}
