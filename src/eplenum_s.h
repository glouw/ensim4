struct eplenum_s
{
    struct chamber_s chamber;
    size_t wave_index;
    thrd_t thread;
    bool use_cfd;
    double pipe_length_m;
    double mic_position_ratio;
    double velocity_low_pass_cutoff_frequency_hz;
};

static int
run_eplenum_wave_thread(void* argument)
{
    struct eplenum_s* self = argument;
    batch_wave(self->wave_index, self->use_cfd, self->pipe_length_m, self->mic_position_ratio, self->velocity_low_pass_cutoff_frequency_hz);
    return 0;
}

static void
launch_eplenum_wave_thread(struct eplenum_s* self)
{
    thrd_create(&self->thread, run_eplenum_wave_thread, self);
}

static void
wait_for_eplenum_wave_thread(struct eplenum_s* self)
{
    thrd_join(self->thread, nullptr);
}
