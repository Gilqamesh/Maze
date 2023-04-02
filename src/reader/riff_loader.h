#pragma once

#include "reader_defs.h"

#pragma pack(push, 1)
struct riff__regular_chunk {
    char   identifier[4];
    u32    data_size;
    void*  data;
};

struct riff__riff_chunk {
    char   identifier[4];
    u32    data_size;
    char   form_type[4];
    void*  data;
};
#pragma pack(pop)

struct riff_loader {
    struct riff__riff_chunk  chunk;
};

enum riff_loader_chunk_type {
    RIFF_LOADER_REGULAR_CHUNK,
    RIFF_LOADER_RIFF_CHUNK
};

DLLEXPORT bool riff_loader__create(struct riff_loader* self, const char* riff_file_path);
DLLEXPORT void riff_loader__destroy(struct riff_loader* self);

DLLEXPORT bool riff_loader__parse_regular_chunk(void* in_data, void* out_chunk);
DLLEXPORT bool riff_loader__parse_riff_chunk(void* in_data, void* out_chunk);

DLLEXPORT enum riff_loader_chunk_type riff_loader__parse_type(void* in_data);
