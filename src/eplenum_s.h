struct eplenum_s
{
    struct chamber_s chamber;
    size_t wave_index;
    thrd_t thread;
    bool use_cfd;
    double pipe_length_m;
};

int
run_eplenum_wave_thread(void* argument)
{
    struct eplenum_s* self = argument;
    batch_wave(self->wave_index, self->use_cfd, self->pipe_length_m);
    return 0;
}

void
launch_eplenum_wave_thread(struct eplenum_s* self)
{
    thrd_create(&self->thread, run_eplenum_wave_thread, self);
}

void
wait_for_eplenum_wave_thread(struct eplenum_s* self)
{
    thrd_join(self->thread, nullptr);
}
