#include "wav_loader.h"

#include "riff_loader.h"

static bool riff_loader_sanitize_format(struct wav_format_chunk* chunk) {
    if (chunk->format_chunk_size != sizeof(*chunk->format)) {
        return false;
    }

    if (strncmp(chunk->format_header, "fmt ", 4) != 0) {
        return false;
    }

    return true;
}

static bool riff_loader_sanitize_data(struct wav_data_chunk* chunk) {
    if (strncmp(chunk->identifier, "data", 4) != 0) {
        return false;
    }

    return true;
}

bool wav_loader_parse(struct wav_loader* self) {
    if (strncmp(self->riff_loader.chunk.form_type, "WAVE", 4) != 0) {
        return false;
    }

    void* data = self->riff_loader.chunk.data;
    if (self->riff_loader.chunk.data_size < 4) {
        return false;
    }
    u32 remaining_size_to_parse = self->riff_loader.chunk.data_size - 4;

    if (riff_loader__parse_regular_chunk(self->riff_loader.chunk.data, (struct riff__regular_chunk*) &self->format) == false) {
        return false;
    }
    u32 parsed_bytes = 8 + self->format.format_chunk_size;
    remaining_size_to_parse -= parsed_bytes;
    (char*) data += parsed_bytes;

    if (riff_loader_sanitize_format(&self->format) == false) {
        return false;
    }

    while (remaining_size_to_parse > 0) {
        if (riff_loader__parse_type(data) == RIFF_LOADER_REGULAR_CHUNK) {
            struct riff__regular_chunk chunk;
            if (riff_loader__parse_regular_chunk(data, &chunk) == false) {
                return false;
            }
            if (remaining_size_to_parse < 8 ||
                chunk.data_size > remaining_size_to_parse - 8
            ) {
                return false;
            }
            parsed_bytes = 8 + chunk.data_size;
            remaining_size_to_parse -= parsed_bytes;
            (char*) data += parsed_bytes;

            if (strncmp(chunk.identifier, "data", 4) == 0) {
                memcpy(&self->samples, &chunk, sizeof(chunk));
            }
        } else {
            struct riff__riff_chunk chunk;
            if (riff_loader__parse_riff_chunk(data, &chunk) == false) {
                return false;
            }
            if (remaining_size_to_parse < 12) {
                return false;
            }
            parsed_bytes = 12;
            remaining_size_to_parse -= parsed_bytes;
            (char*) data += parsed_bytes;
        }
    }

    return true;
}

bool wav_loader__create(struct wav_loader* self, const char* wav_file_path) {;
    if (riff_loader__create(&self->riff_loader, wav_file_path) == false) {
        return false;
    }

    if (wav_loader_parse(self) == false) {
        riff_loader__destroy(&self->riff_loader);
        return false;
    }

    return true;
}

void wav_loader__destroy(struct wav_loader* self) {
    riff_loader__destroy(&self->riff_loader);
}
