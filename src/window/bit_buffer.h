#pragma once

#include "window_defs.h"
#include "../math/v2u32.h"

struct bit_buffer {
    void          *buffer;
    BITMAPINFO    info;
    struct v2u32  dims;
};

bool bit_buffer__create(struct bit_buffer* self, struct v2u32 dims);
void bit_buffer__destroy(struct bit_buffer* self);

void bit_buffer__blit_to_window(struct bit_buffer* self, HDC window_device_context, struct v2u32 window_dims);

bool bit_buffer__resize(struct bit_buffer* self, struct v2u32 new_dims);
