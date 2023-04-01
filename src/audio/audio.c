#include "audio.h"

#include "../math/clamp.h"

bool audio__init_module(void) {
    if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED))) {
        return false;
    }

    return true;
}

void audio__deinit_module(void) {
    CoUninitialize();
}

#define SOURCE_VOICE_MAX_FREQUENCY_RATIO XAUDIO2_DEFAULT_FREQ_RATIO

bool audio__create(struct audio* self, const char* audio_file_path, bool should_loop_forever) {
    if (FAILED(XAudio2Create(&self->audio_interface, 0, XAUDIO2_DEFAULT_PROCESSOR))) {
        return false;
    }

    u32 number_of_channels = XAUDIO2_DEFAULT_CHANNELS;
    number_of_channels = clamp__u32(0, number_of_channels, XAUDIO2_MAX_AUDIO_CHANNELS);
    u32 input_sample_rate = XAUDIO2_DEFAULT_SAMPLERATE;
    AUDIO_STREAM_CATEGORY stream_category = AudioCategory_GameEffects;
    wchar_t* device_id = NULL;
    XAUDIO2_EFFECT_CHAIN* effect_chain = NULL;

    if (FAILED(self->audio_interface->lpVtbl->CreateMasteringVoice(
        self->audio_interface,
        &self->mastering_voice,
        number_of_channels,
        input_sample_rate,
        0,
        device_id,
        effect_chain,
        stream_category
    ))) {
        return false;
    }

    WAVEFORMATEX wfx = {0};
    memset(&self->source_buffer_config, 0, sizeof(self->source_buffer_config));

    if (wav_loader__create(&self->wav_loader, audio_file_path) == false) {
        return false;
    }

    struct wav_format* format = self->wav_loader.format.format;

    wfx.wFormatTag      = format->format_type;
    wfx.nChannels       = format->number_of_channels;
    wfx.nSamplesPerSec  = format->sample_rate;
    wfx.nAvgBytesPerSec = format->byte_rate;
    wfx.nBlockAlign     = format->block_align;
    wfx.wBitsPerSample  = format->bits_per_sample;
    wfx.cbSize          = 0;

    self->source_buffer_config.AudioBytes = self->wav_loader.samples.data_size;
    self->source_buffer_config.pAudioData = self->wav_loader.samples.data;
    self->source_buffer_config.Flags      = XAUDIO2_END_OF_STREAM;

    if (should_loop_forever) {
        self->source_buffer_config.LoopCount = XAUDIO2_LOOP_INFINITE;
    }

    // note: submix voice
    const u32 submix_input_channels = 1;
    const u32 submix_input_sample_rate = format->sample_rate;
    const u32 submix_flags = 0;
    const u32 submix_processing_stage = 0;
    const XAUDIO2_VOICE_SENDS* submix_send_list = NULL;
    const XAUDIO2_EFFECT_CHAIN* submix_effect_chain = NULL;
    if (FAILED(self->audio_interface->lpVtbl->CreateSubmixVoice(
        self->audio_interface,
        &self->main_submix_voice,
        submix_input_channels,
        submix_input_sample_rate,
        submix_flags,
        submix_processing_stage,
        submix_send_list,
        submix_effect_chain
    ))) {
        return false;
    }

    XAUDIO2_SEND_DESCRIPTOR sfx_send = { 0 };
    sfx_send.pOutputVoice = (IXAudio2Voice *) self->main_submix_voice;

    XAUDIO2_VOICE_SENDS sfx_send_list = { 0 };
    sfx_send_list.SendCount = 1;
    sfx_send_list.pSends = &sfx_send;

    // note: source voice
    const u32 source_voice_flags = 0;
    const r32 max_frequency_ratio = SOURCE_VOICE_MAX_FREQUENCY_RATIO;
    IXAudio2VoiceCallback* source_voice_callback = NULL;
    const XAUDIO2_VOICE_SENDS* source_voice_send_list = &sfx_send_list;
    const XAUDIO2_EFFECT_CHAIN* source_voice_effect_chain = NULL;
    if (FAILED(self->audio_interface->lpVtbl->CreateSourceVoice(
        self->audio_interface,
        &self->source_voice,
        &wfx,
        source_voice_flags,
        max_frequency_ratio,
        source_voice_callback,
        source_voice_send_list,
        source_voice_effect_chain
    ))) {
        wav_loader__destroy(&self->wav_loader);
        return false;
    }

    const XAUDIO2_BUFFER_WMA* additional_wma_data = NULL;
    if (FAILED(self->source_voice->lpVtbl->SubmitSourceBuffer(
        self->source_voice,
        &self->source_buffer_config,
        additional_wma_data
    ))) {
        wav_loader__destroy(&self->wav_loader);
        return false;
    }

    return true;
}

void audio__destroy(struct audio* self) {
    wav_loader__destroy(&self->wav_loader);
}

bool audio__start(struct audio* self) {
    const u32 operation_set = XAUDIO2_COMMIT_NOW;
    if (FAILED(self->source_voice->lpVtbl->Start(
        self->source_voice,
        0,
        operation_set
    ))) {
        return false;
    }

    return true;
}

static inline r32 audio_decibels_to_volume(r32 decibels) {
    return powf(10.0f, decibels / 20.0f);
}

static inline r32 audio_volume_to_decibels(r32 volume) {
    if (volume == 0.0f) {
        return 0.0f;
        return -3.402823466e+38f;
    }
    return 20.0f * log10f(volume);
}

bool audio__set_volume(struct audio* self, r32 decibels) {
    u32 operation_set = XAUDIO2_COMMIT_NOW;
    r32 volume = audio_decibels_to_volume(decibels);
    volume = clamp__r32(-XAUDIO2_MAX_VOLUME_LEVEL, volume, XAUDIO2_MAX_VOLUME_LEVEL);
    if (FAILED(self->main_submix_voice->lpVtbl->SetVolume(
        self->main_submix_voice,
        volume,
        operation_set
    ))) {
        return false;
    }

    return true;
}

r32 audio__get_volume(struct audio* self) {
    r32 volume;
    self->main_submix_voice->lpVtbl->GetVolume(self->main_submix_voice, &volume);
    return audio_volume_to_decibels(volume);
}

bool audio__set_frequency_ratio(struct audio* self, r32 frequency_ratio) {
    u32 operation_set = XAUDIO2_COMMIT_NOW;
    frequency_ratio = clamp__r32(XAUDIO2_MIN_FREQ_RATIO, frequency_ratio, SOURCE_VOICE_MAX_FREQUENCY_RATIO);
    if (FAILED(self->source_voice->lpVtbl->SetFrequencyRatio(
        self->source_voice,
        frequency_ratio,
        operation_set
    ))) {
        return false;
    }

    return true;
}

r32 audio__get_frequency_ratio(struct audio* self) {
    r32 frequency_ratio;
    self->source_voice->lpVtbl->GetFrequencyRatio(self->source_voice, &frequency_ratio);
    return frequency_ratio;
}
