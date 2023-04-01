#include "bmp_loader.h"

static inline bool _bmp_loader_ensure_size(struct bmp_loader* self, u32 size) {
    if (self->size < size) {
        self->size <<= 1;
        self->data = (char*) realloc(self->data, self->size * sizeof(*self->data));
        if (self->data == NULL) {
            return false;
        }
    }

    return true;
}

struct bit_scan_result {
    bool found;
    u32  index;
};
static inline struct bit_scan_result _find_least_significant_set_bit(u32 value) {
    struct bit_scan_result result = { 0 };

    for(u32 test = 0; test < 32; ++test) {
        if (value & (1 << test)) {
            result.index = test;
            result.found = true;
            break;
        }
    }
    
    return result;
}

static inline u32 _rotate_left(i32 value, i32 amount) {
    amount &= 31;
    u32 result = ((value << amount) | (value >> (32 - amount)));

    return result;
}

bool bmp_loader__create(struct bmp_loader* self, struct file_reader* reader, struct bitmap* out) {
    self->size   = 1024;
    self->data   = (char*) malloc(self->size * sizeof(*self->data));
    self->fill   = 0;

    u32 read_bytes_so_far = 0;
    while (file_reader__eof_reached(reader) == false) {
        char read_byte;
        if (file_reader__read(reader, &read_byte, sizeof(read_byte)) < sizeof(read_byte)) {
            return false;
        }

        if (_bmp_loader_ensure_size(self, ++self->fill) == false) {
            return false;
        }

        self->data[self->fill - 1] = read_byte;
    }

    struct bitmap_header* header = (struct bitmap_header*) self->data;

    out->data = (u32*) ((u8*) self->data + header->bitmap_offset);
    out->dims = v2u32(header->width, header->height);

    u32 green_mask = header->green_mask;
    u32 red_mask   = header->red_mask;
    u32 blue_mask  = header->blue_mask;
    u32 alpha_mask = ~(red_mask | green_mask | blue_mask);

    struct bit_scan_result red_scan = _find_least_significant_set_bit(red_mask);
    struct bit_scan_result green_scan = _find_least_significant_set_bit(green_mask);
    struct bit_scan_result blue_scan = _find_least_significant_set_bit(blue_mask);
    struct bit_scan_result alpha_scan = _find_least_significant_set_bit(alpha_mask);

    i32 red_shift = 16 - (i32)red_scan.index;
    i32 green_shift = 8 - (i32)green_scan.index;
    i32 blue_shift = 0 - (i32)blue_scan.index;
    i32 alpha_shift = 24 - (i32)alpha_scan.index;

    u8* bmp_p = (u8*) out->data;

    for (u32 row = 0; row < header->height; ++row) {
        for (u32 col = 0; col < header->width; ++col) {
            u32 color = *bmp_p;

            *bmp_p++ = (
                _rotate_left(color & red_mask, red_shift) |
                _rotate_left(color & green_mask, green_shift) |
                _rotate_left(color & blue_mask, blue_shift) |
                _rotate_left(color & alpha_mask, alpha_shift)
            );
        }
    }

    return true;
}

void bmp_loader__destroy(struct bmp_loader* self) {
    if (self->data) {
        free(self->data);
    }
}
