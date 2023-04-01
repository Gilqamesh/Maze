#pragma once

#include "audio_defs.h"

#include "../reader/wav_loader.h"

#include <xaudio2.h>

struct audio {
    IXAudio2*                audio_interface;
    IXAudio2SourceVoice*     source_voice;
    IXAudio2SubmixVoice*     main_submix_voice;
    IXAudio2MasteringVoice*  mastering_voice;
    XAUDIO2_BUFFER           source_buffer_config;
    struct wav_loader        wav_loader;
};

DLLEXPORT bool audio__init_module(void);
DLLEXPORT void audio__deinit_module(void);

DLLEXPORT bool audio__create(struct audio* self, const char* audio_file_path, bool should_loop_forever);
DLLEXPORT void audio__destroy(struct audio* self);

DLLEXPORT bool audio__start(struct audio* self);

// @param decibels in the interval of [0, 144.5]
DLLEXPORT bool audio__set_volume(struct audio* self, r32 decibels);
DLLEXPORT r32 audio__get_volume(struct audio* self);

// @param frequency in the interval of [0.0005, 2.0]
DLLEXPORT bool audio__set_frequency_ratio(struct audio* self, r32 frequency_ratio);
DLLEXPORT r32 audio__get_frequency_ratio(struct audio* self);
