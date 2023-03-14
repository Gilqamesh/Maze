#include "file_reader.h"

bool file_reader__create(struct file_reader* self, struct console* console, const char* filename) {
    self->console            = console;
    self->read_bytes_so_far  = 0;
    self->available          = sizeof(self->buffer);
    self->head               = 0;
    self->tail               = 0;

    self->file_handle        = CreateFileA(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    self->eof                = false;

    if (self->file_handle == INVALID_HANDLE_VALUE) {
        console__fatal(self->console, "in 'file_reader__create': CreateFileA failed to open file: %s", filename);
    }

    return true;
}

void file_reader__destroy(struct file_reader* self) {
    if (CloseHandle(self->file_handle) == 0) {
        console__fatal(self->console, "in 'file_reader__destroy': failed to close file handle, GetLastError = %d", GetLastError());
    }
}

bool file_reader__eof_reached(struct file_reader* self) {
    return self->eof;
}

static int file_reader__size(struct file_reader* self) {
    return sizeof(self->buffer) - self->available;
}

static inline int file_reader__available(struct file_reader *self) {
    return self->available;
}

static void file_reader_store_byte(struct file_reader* self, byte byte) {
    self->buffer[self->head++] = byte;
    --self->available;

    if (self->head == sizeof(self->buffer)) {
        self->head = 0;
    }
}

static byte file_reader_read_byte(struct file_reader* self) {
    byte byte = self->buffer[self->tail++];
    ++self->available;

    if (self->tail == sizeof(self->buffer)) {
        self->tail = 0;
    }

    self->read_bytes_so_far += 1;

    return byte;
}

static int file_reader__read(struct file_reader *self, int size) {
    if (file_reader__available(self) < size) {
        console__log(self->console, "in 'file_reader__read': cannot read %d much, only %d is available", size, file_reader__available(self));
        size = file_reader__available(self);
    }

    byte buffer[sizeof(self->buffer)];
    u32 read_amount;
    if (ReadFile(self->file_handle, buffer, size, &read_amount, NULL) == FALSE) {
        console__fatal(self->console, "in 'file_reader__read': ReadFile failed");
    }

    for (int buffer_index = 0; buffer_index < read_amount; ++buffer_index) {
        file_reader_store_byte(self, buffer[buffer_index]);
    }

    return read_amount;
}

void file_reader__read_byte(struct file_reader* self, void* out) {
    if (file_reader__eof_reached(self)) {
        console__fatal(self->console, "in 'file_reader__read_byte': already reached end of file, nothing to read");
        return ;
    }

    if (file_reader__size(self) == 0) {
        file_reader__read(self, file_reader__available(self));
    }

    *(byte*)out = file_reader_read_byte(self);

    if (file_reader__size(self) == 0 && file_reader__read(self, 1) == 0) {
        self->eof = true;
    }
}

void file_reader__read_word(struct file_reader* self, void* out) {
    file_reader__read_byte(self, (byte*)out);
    file_reader__read_byte(self, (byte*)out + 1);
}

uint32_t file_reader__read_bytes_so_far(struct file_reader* self) {
    return self->read_bytes_so_far;
}
