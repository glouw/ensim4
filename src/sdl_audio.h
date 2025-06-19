static SDL_AudioStream* g_sdl_audio_stream = nullptr;
static SDL_AudioSpec g_sdl_audio_spec = {};

static void
init_sdl_audio()
{
    g_sdl_audio_spec.channels = 1;
    g_sdl_audio_spec.format = SDL_AUDIO_F32;
    g_sdl_audio_spec.freq = g_std_audio_sample_rate_hz;
    g_sdl_audio_stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &g_sdl_audio_spec, nullptr, nullptr);
    SDL_ResumeAudioStreamDevice(g_sdl_audio_stream);
}

static void
exit_sdl_audio()
{
    SDL_DestroyAudioStream(g_sdl_audio_stream);
}

static size_t
get_audio_buffer_size()
{
    return SDL_GetAudioStreamQueued(g_sdl_audio_stream) / sizeof(float);
}

static void
buffer_audio(struct synth_s* synth)
{
    SDL_PutAudioStreamData(g_sdl_audio_stream, synth->value, synth->index * sizeof(float));
}
