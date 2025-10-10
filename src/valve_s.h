struct valve_s
{
    double engage_r;
    double ramp_r;
};

static double
calc_valve_nozzle_open_ratio(struct valve_s* self, struct crankshaft_s* crankshaft)
{
    double otto_theta_r = fmod(crankshaft->theta_r, g_std_four_pi_r);
    double otto_engage_r = fmod(self->engage_r, g_std_four_pi_r);
    if(otto_engage_r < 0.0)
    {
        otto_engage_r += g_std_four_pi_r;
    }
    if(otto_theta_r < otto_engage_r)
    {
        otto_theta_r += g_std_four_pi_r;
    }
    double open_r = otto_theta_r - otto_engage_r;
    double term1 = 35.0 * pow(open_r / self->ramp_r, 4.0);
    double term2 = 84.0 * pow(open_r / self->ramp_r, 5.0);
    double term3 = 70.0 * pow(open_r / self->ramp_r, 6.0);
    double term4 = 20.0 * pow(open_r / self->ramp_r, 7.0);
    double valve_nozzle_open_ratio = clamp(term1 - term2 + term3 - term4, 0.0, 1.0);
    return valve_nozzle_open_ratio;
}
