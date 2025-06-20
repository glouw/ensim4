static constexpr size_t g_starter_harmonics = 10;
static constexpr size_t g_starter_generic_tooth_count = 10;

struct starter_s
{
    double rated_torque_n_m;
    double no_load_angular_velocity_r_per_s;
    double radius_m;
};

static double
calc_flywheel_to_starter_gear_ratio(
    const struct starter_s* self,
    const struct flywheel_s* flywheel)
{
    double starter_gear_ratio = flywheel->radius_m / self->radius_m;
    return starter_gear_ratio;
}

static double
calc_starter_angular_velocity_r_per_s(
    const struct starter_s* self,
    const struct flywheel_s* flywheel,
    const struct crankshaft_s* crankshaft)
{
    double gear_ratio = calc_flywheel_to_starter_gear_ratio(self, flywheel);
    double starter_angular_velocity_r_per_s = max(crankshaft->angular_velocity_r_per_s * gear_ratio, 0.0);
    return starter_angular_velocity_r_per_s;
}

static double
calc_starter_torque_on_flywheel_n_m(
    const struct starter_s* self,
    const struct flywheel_s* flywheel,
    const struct crankshaft_s* crankshaft)
{
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

static double
calc_starter_gear_audio_sample(
   const struct starter_s* self,
   const struct flywheel_s* flywheel,
   const struct crankshaft_s* crankshaft)
{
   double gear_ratio = calc_flywheel_to_starter_gear_ratio(self, flywheel);
   double starter_theta_r = crankshaft->theta_r * gear_ratio;
   double gear_audio_sample = 0.0;
   for(size_t harmonic = 1; harmonic <= g_starter_harmonics; harmonic++)
   {
       double harmonic_amplitude = 1.0 / harmonic;
       double tooth_phase = starter_theta_r * g_starter_generic_tooth_count * harmonic;
       gear_audio_sample += harmonic_amplitude * sin(tooth_phase);
   }
   return gear_audio_sample;
}
