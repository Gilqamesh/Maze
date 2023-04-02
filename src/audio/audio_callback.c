#include "audio_callback.h"
#include "audio.h"

void audio_callback__create(
    struct audio_callback* self,
    void (*on_buffer_end)(struct IXAudio2VoiceCallback* native_self, void* buffer_context),
    void (*on_buffer_start)(struct IXAudio2VoiceCallback* native_self, void* buffer_context),
    void (*on_loop_end)(struct IXAudio2VoiceCallback* native_self, void* buffer_context),
    void (*on_stream_end)(struct IXAudio2VoiceCallback* native_self),
    void (*on_voice_error)(struct IXAudio2VoiceCallback* native_self, void* buffer_context, HRESULT error_code),
    void (*on_voice_processing_pass_end)(struct IXAudio2VoiceCallback* native_self),
    void (*on_voice_processing_pass_start)(struct IXAudio2VoiceCallback* native_self, u32 bytes_required)
) {
    self->callback.lpVtbl = &self->callback_this;
    self->callback_this.OnBufferEnd = on_buffer_end;
    self->callback_this.OnBufferStart = on_buffer_start;
    self->callback_this.OnLoopEnd = on_loop_end;
    self->callback_this.OnStreamEnd = on_stream_end;
    self->callback_this.OnVoiceError = on_voice_error;
    self->callback_this.OnVoiceProcessingPassEnd = on_voice_processing_pass_end;
    self->callback_this.OnVoiceProcessingPassStart = on_voice_processing_pass_start;
}

void audio_callback__on_buffer_end__default(struct IXAudio2VoiceCallback* self, void* buffer_context) {
    struct audio* audio = (struct audio *) buffer_context;

    // u32 flags = XAUDIO2_PLAY_TAILS;
    // u32 flags = 0;
    // u32 operation_set = XAUDIO2_COMMIT_NOW;
    // audio->source_voice->lpVtbl->Stop(
    //     audio->source_voice,
    //     flags,
    //     operation_set
    // );
}

void audio_callback__on_buffer_start__default(struct IXAudio2VoiceCallback* self, void* buffer_context) {
    struct audio* audio = (struct audio*) buffer_context;

//     const XAUDIO2_BUFFER_WMA* additional_wma_data = NULL;
//     audio->source_voice->lpVtbl->SubmitSourceBuffer(
//         audio->source_voice,
//         &audio->source_buffer_config,
//         additional_wma_data
//     );
}

void audio_callback__on_loop_end__default(struct IXAudio2VoiceCallback* self, void* buffer_context) {
}

void audio_callback__on_stream_end__default(struct IXAudio2VoiceCallback* self) {
}

void audio_callback__on_voice_error__default(struct IXAudio2VoiceCallback* self, void* buffer_context, HRESULT error_code) {
}

void audio_callback__on_voice_processing_pass_end__default(struct IXAudio2VoiceCallback* self) {
}

void audio_callback__on_voice_processing_pass_start__default(struct IXAudio2VoiceCallback* self, u32 bytes_required) {
}
