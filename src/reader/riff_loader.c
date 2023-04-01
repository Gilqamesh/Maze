#include "riff_loader.h"

#include "file_reader.h"

bool riff_loader_init_riff_chunk(struct riff_loader* self, struct file_reader* file_reader) {
    struct riff__riff_chunk* chunk = &self->chunk;

    if (file_reader__eof_reached(file_reader) ||
        file_reader__read(file_reader, &chunk->identifier, sizeof(chunk->identifier)) < sizeof(chunk->identifier)
    ) {
        return false;
    }

    if (strncmp(chunk->identifier, "RIFF", 4) != 0) {
        return false;
    }

    if (file_reader__eof_reached(file_reader) ||
        file_reader__read(file_reader, &chunk->data_size, sizeof(chunk->data_size)) < sizeof(chunk->data_size)
    ) {
        return false;
    }

    if (chunk->data_size < 4) {
        return false;
    }

    if ((chunk->data = malloc(chunk->data_size - 4)) == NULL) {
        return false;
    }

    if (file_reader__eof_reached(file_reader) ||
        file_reader__read(file_reader, chunk->form_type, sizeof(chunk->form_type)) < sizeof(chunk->form_type)
    ) {
        riff_loader__destroy(self);
        return false;
    }

    if (file_reader__eof_reached(file_reader) ||
        file_reader__read(file_reader, chunk->data, chunk->data_size - 4) < chunk->data_size - 4
    ) {
        riff_loader__destroy(self);
        return false;
    }

    if (chunk->data_size & 1 &&
        (file_reader__eof_reached(file_reader) ||
        file_reader__read(file_reader, NULL, 1) < 1)
    ) {
        riff_loader__destroy(self);
        return false;
    }

    return true;
}

bool riff_loader__create(struct riff_loader* self, const char* riff_file_path) {
    struct file_reader file_reader;
    if (file_reader__create(&file_reader, riff_file_path) == false) {
        return false;
    }

    if (riff_loader_init_riff_chunk(self, &file_reader) == false) {
        return false;
    }

    file_reader__destroy(&file_reader);

    return true;
}

void riff_loader__destroy(struct riff_loader* self) {
    if (self->chunk.data != NULL) {
        free(self->chunk.data);
        self->chunk.data = NULL;
    }
}

static void riff_loader__parse_header(void* in_data, void* out_chunk) {
    memcpy(out_chunk, in_data, 4); // id
    memcpy((char*) out_chunk + 4, (char*) in_data + 4, 4); // data_size
    if (*((u32*) ((char*) out_chunk + 4)) & 1) {
        (*((u32*) ((char*) out_chunk + 4))) += 1;
    }
}

static bool inline riff_loader__is_riff_chunk(char* id) {
    return strncmp(id, "RIFF", 4) == 0 || strncmp(id, "LIST", 4) == 0;
}



bool riff_loader__parse_regular_chunk(void* in_data, void* out_chunk) {
    struct riff__regular_chunk* chunk = (struct riff__regular_chunk*) out_chunk;

    riff_loader__parse_header(in_data, chunk);
    if (riff_loader__is_riff_chunk(in_data)) {
        return false;
    }
    chunk->data = (char*) in_data + 8;

    return true;
}

bool riff_loader__parse_riff_chunk(void* in_data, void* out_chunk) {
    struct riff__riff_chunk* chunk = (struct riff__riff_chunk*) out_chunk;

    riff_loader__parse_header(in_data, chunk);
    if (riff_loader__is_riff_chunk(in_data) == false) {
        return false;
    }
    memcpy(chunk->form_type, (char*) in_data + 8, sizeof(chunk->form_type)); // form_type
    chunk->data = (char*) in_data + 12;

    return true;
}

enum riff_loader_chunk_type riff_loader__parse_type(void* in_data) {
    if (riff_loader__is_riff_chunk(in_data)) {
        return RIFF_LOADER_RIFF_CHUNK;
    } else {
        return RIFF_LOADER_REGULAR_CHUNK;
    }
}
