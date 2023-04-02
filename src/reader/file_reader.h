#pragma once

#include "reader_defs.h"

struct file_reader { // circular buffer data structure for storing read bytes from a file 
    HANDLE            file_handle;
    u32               read_bytes_so_far;
    u32               available_to_store;
    u32               head;
    u32               tail;
    bool              eof;
    char              buffer[4096];
};

DLLEXPORT bool file_reader__create(struct file_reader* self, const char* filename);
DLLEXPORT void file_reader__destroy(struct file_reader* self);

// todo: remove
DLLEXPORT bool file_reader__eof_reached(struct file_reader* self);

DLLEXPORT u32 file_reader__read(struct file_reader* self, void* out, u32 size);

enum FILE_READER_ERROR {
    FILE_READER_ERROR_REACHED_EOF = ERROR_START_CODE_FILE_READER,
    FILE_READER_ERROR_READFILE_FAILED
};
