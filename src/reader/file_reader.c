#include "file_reader.h"

#include "../math/clamp.h"

bool file_reader__create(struct file_reader* self, const char* filename) {
    self->available_to_store = sizeof(self->buffer);
    self->head               = 0;
    self->tail               = 0;

    self->file_handle        = CreateFileA(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    self->eof                = false;

    if (self->file_handle == INVALID_HANDLE_VALUE) {
        return false;
    }

    return true;
}

void file_reader__destroy(struct file_reader* self) {
    CloseHandle(self->file_handle);
}

static inline u32 file_reader_available_to_store(struct file_reader *self) {
    return self->available_to_store;
}

bool file_reader__eof_reached(struct file_reader* self) {
    if (self->eof && file_reader_available_to_store(self) == sizeof(self->buffer)) {
        return true;
    }

    return false;
}

static u32 file_reader_size(struct file_reader* self) {
    return sizeof(self->buffer) - self->available_to_store;
}

static u32 file_reader_read_from_buffer(struct file_reader* self, void* out, u32 size) {
    u32 read_bytes = 0;
    size = clamp__u32(0, size, sizeof(self->buffer));
    size = clamp__u32(0, size, file_reader_size(self));
    u32 first_half_size = clamp__u32(0, size, sizeof(self->buffer) - self->tail);
    if (out != NULL) {
        memcpy(out, self->buffer + self->tail, first_half_size);
    }
    read_bytes += first_half_size;
    self->tail += first_half_size;
    size -= first_half_size;
    if (self->tail == sizeof(self->buffer)) {
        if (out != NULL) {
            memcpy((char*) out + first_half_size, self->buffer, size);
        }
        self->tail = size;
        read_bytes += size;
    }

    self->available_to_store += read_bytes;

    return read_bytes;
}

static u32 file_reader_read_from_file(struct file_reader *self, u32 size) {
    if (self->eof) {
        ExitProcess(FILE_READER_ERROR_REACHED_EOF);
    }
    size = clamp__u32(0, size, file_reader_available_to_store(self));

    u32 first_half_size = clamp__u32(0, size, sizeof(self->buffer) - self->head);
    u32 stored_bytes = 0;
    u32 read_bytes;
    if (ReadFile(self->file_handle, self->buffer + self->head, first_half_size, &read_bytes, NULL) == FALSE) {
        ExitProcess(FILE_READER_ERROR_READFILE_FAILED);
    }
    if (read_bytes < first_half_size) {
        self->eof = true;
    }
    self->head += read_bytes;
    size -= read_bytes;
    stored_bytes += read_bytes;
    if (self->head == sizeof(self->buffer)) {
        self->head = 0;
        if (size > 0 && self->eof != true) {
            if (ReadFile(self->file_handle, self->buffer, size, &read_bytes, NULL) == FALSE) {
                ExitProcess(FILE_READER_ERROR_READFILE_FAILED);
            }
            if (read_bytes < size) {
                self->eof = true;
            }

            self->head = read_bytes;
            stored_bytes += read_bytes;

        }
    }

    self->available_to_store -= stored_bytes;

    return stored_bytes;
}

u32 file_reader__read(struct file_reader* self, void* out, u32 size) {
    u32 stored_out_bytes_count = 0;
    while (size > 0 && file_reader__eof_reached(self) == false) {
        if (file_reader_available_to_store(self) == sizeof(self->buffer)) {
            file_reader_read_from_file(self, sizeof(self->buffer));
        }

        u32 read_from_buffer_count;
        if (out != NULL) {
            read_from_buffer_count = file_reader_read_from_buffer(self, (char*) out + stored_out_bytes_count, size);
        } else {
            read_from_buffer_count = file_reader_read_from_buffer(self, out, size);
        }

        stored_out_bytes_count += read_from_buffer_count;
        size -= read_from_buffer_count;
    }

    return stored_out_bytes_count;
}
