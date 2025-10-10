struct crankshaft_s
{
    double theta_r;
    double angular_velocity_r_per_s;
    double mass_kg;
    double radius_m;
};

static void
accelerate_crankshaft(struct crankshaft_s* self, double angular_acceleration_r_per_s2)
{
    self->angular_velocity_r_per_s += angular_acceleration_r_per_s2 * g_std_dt_s;
}

static void
turn_crankshaft(struct crankshaft_s* self)
{
    self->theta_r += self->angular_velocity_r_per_s * g_std_dt_s;
}

static double
calc_crankshaft_moment_of_inertia_kg_m2(struct crankshaft_s* self)
{
    return 0.5 * self->mass_kg * pow(self->radius_m, 2.0);
}
