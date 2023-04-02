#pragma once

#include "audio_defs.h"

struct audio_callback {
    IXAudio2VoiceCallbackVtbl  callback_this;
    IXAudio2VoiceCallback      callback;
};

DLLEXPORT void audio_callback__create(
    struct audio_callback* self,
    void (*on_buffer_end)(struct IXAudio2VoiceCallback* native_self, void* buffer_context),
    void (*on_buffer_start)(struct IXAudio2VoiceCallback* native_self, void* buffer_context),
    void (*on_loop_end)(struct IXAudio2VoiceCallback* native_self, void* buffer_context),
    void (*on_stream_end)(struct IXAudio2VoiceCallback* native_self),
    void (*on_voice_error)(struct IXAudio2VoiceCallback* native_self, void* buffer_context, HRESULT error_code),
    void (*on_voice_processing_pass_end)(struct IXAudio2VoiceCallback* native_self),
    void (*on_voice_processing_pass_start)(struct IXAudio2VoiceCallback* native_self, u32 bytes_required)
);

DLLEXPORT void audio_callback__on_buffer_end__default(struct IXAudio2VoiceCallback* self, void* buffer_context);

DLLEXPORT void audio_callback__on_buffer_start__default(struct IXAudio2VoiceCallback* self, void* buffer_context);

DLLEXPORT void audio_callback__on_loop_end__default(struct IXAudio2VoiceCallback* self, void* buffer_context);

DLLEXPORT void audio_callback__on_stream_end__default(struct IXAudio2VoiceCallback* self);

DLLEXPORT void audio_callback__on_voice_error__default(struct IXAudio2VoiceCallback* self, void* buffer_context, HRESULT error_code);

DLLEXPORT void audio_callback__on_voice_processing_pass_end__default(struct IXAudio2VoiceCallback* self);

DLLEXPORT void audio_callback__on_voice_processing_pass_start__default(struct IXAudio2VoiceCallback* self, u32 bytes_required);
