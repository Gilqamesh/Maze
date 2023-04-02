#pragma once

#include "reader_defs.h"

#include "riff_loader.h"

#pragma pack(push, 1)
struct wav_format {
    u16  format_type; // 1 (PCM)
    u16  number_of_channels; // ex. 2
    u32  sample_rate; // ex. 44100 (CD)
    u32  byte_rate; // bytes/s, calculated as (sample_rate * bits_per_sample * channels) / 8
    u16  block_align; // number of bytes per sample, calculated as (bits_per_sample * channels) / 8
    u16  bits_per_sample;
};

struct wav_format_chunk {
    char                format_header[4]; // must be "fmt "
    u32                 format_chunk_size; // 16 bytes
    struct wav_format*  format;
};

struct wav_data_chunk { // sample data
    char   identifier[4];
    u32    data_size;
    void*  data;
};
#pragma pack(pop)

struct wav_loader {
    struct riff_loader       riff_loader;
    struct wav_format_chunk  format;
    struct wav_data_chunk    samples;
};

DLLEXPORT bool wav_loader__create(struct wav_loader* self, const char* wav_file_path);
DLLEXPORT void wav_loader__destroy(struct wav_loader* self);
