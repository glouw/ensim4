struct flywheel_s
{
    double mass_kg;
    double radius_m;
};

static double
calc_flywheel_moment_of_inertia_kg_m2(struct flywheel_s* self)
{
    return 0.5 * self->mass_kg * pow(self->radius_m, 2.0);
}
