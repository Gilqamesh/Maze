#pragma once

#include "reader_defs.h"
#include "../console/console.h"

struct file_reader { // circular buffer data structure for storing read bytes from a file 
    struct console*   console;
    HANDLE            file_handle;
    u32               read_bytes_so_far;
    int               available;
    int               head;
    int               tail;
    bool              eof;
    char              buffer[1024];
};

DLLEXPORT bool file_reader__create(struct file_reader* self, struct console* console, const char* filename);
DLLEXPORT void file_reader__destroy(struct file_reader* self);

DLLEXPORT bool file_reader__eof_reached(struct file_reader* self);

DLLEXPORT void file_reader__read_byte(struct file_reader* self, void* out);

DLLEXPORT u32 file_reader__read_bytes_so_far(struct file_reader* self);
