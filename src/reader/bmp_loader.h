#pragma once

#include "reader_defs.h"
#include "file_reader.h"
#include "bitmap.h"

#pragma pack(push, 1)
struct bitmap_header {
    u16 file_type;
    u32 file_size;
    u16 reserved_1;
    u16 reserver_2;
    u32 bitmap_offset;
    u32 size;
    i32 width;
    i32 height;
    u16 planes;
    u16 bits_per_pixel;
    u32 compression;
    u32 size_of_bitmap;
    i32 horz_resolution;
    i32 vert_resolution;
    u32 colors_used;
    u32 colors_important;

    u32 red_mask;
    u32 green_mask;
    u32 blue_mask;
};
#pragma pack(pop)

struct bmp_loader {
    char*                 data;
    u32                   size;
    u32                   fill;
};

DLLEXPORT bool bmp_loader__create(struct bmp_loader* self, struct file_reader* reader, struct bitmap* out);
DLLEXPORT void bmp_loader__destroy(struct bmp_loader* self);
