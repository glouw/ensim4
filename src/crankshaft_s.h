struct crankshaft_s
{
    double theta_r;
    double angular_velocity_r_per_s;
    double friction_coefficient;
};

static void
turn_crankshaft(struct crankshaft_s* self)
{
    self->theta_r += self->angular_velocity_r_per_s * std_dt_s;
}
