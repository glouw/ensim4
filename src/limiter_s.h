struct limiter_s
{
    double cutoff_angular_velocity_r_per_s;
    double relaxed_angular_velocity_r_per_s;
    bool is_limiting;
};

static void
maybe_limit_engine(struct limiter_s* self, struct crankshaft_s* crankshaft, bool* can_ignite)
{
    double delta_r_per_s = self->cutoff_angular_velocity_r_per_s - crankshaft->angular_velocity_r_per_s;
    if(delta_r_per_s < 0.0)
    {
        self->is_limiting = true;
        *can_ignite = false;
    }
    if(self->is_limiting)
    {
        if(delta_r_per_s > self->relaxed_angular_velocity_r_per_s)
        {
            self->is_limiting = false;
            *can_ignite = true;
        }
    }
}
