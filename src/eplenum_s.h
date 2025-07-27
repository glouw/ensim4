struct eplenum_s
{
    struct chamber_s chamber;
    size_t wave_index;
    thrd_t thread;
    bool use_cfd;
};

static int
run_eplenum_wave_thread(void* argument)
{
    struct eplenum_s* self = argument;
    batch_step_wave(self->wave_index, self->use_cfd);
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
