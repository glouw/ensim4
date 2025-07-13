struct eplenum_s
{
    struct chamber_s chamber;
    struct wave_s wave;
    thrd_t thread;
};

static int
run_eplenum_wave_thread(void* argument)
{
    return 0;
}

static void
launch_eplenum_wave_thread(struct eplenum_s* self)
{
    thrd_create(&self->thread, run_eplenum_wave_thread, nullptr);
}

static void
wait_for_eplenum_wave_thread(struct eplenum_s* self)
{
    thrd_join(self->thread, nullptr);
}
