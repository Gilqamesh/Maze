#include "bit_buffer.h"
#include "window.h"

bool bit_buffer__create(struct bit_buffer* self, struct v2u32 dims) {
    self->bytes_per_pixel              = 4;
    self->dims                         = dims;

    self->info.bmiHeader.biPlanes      = 1;
    self->info.bmiHeader.biBitCount    = self->bytes_per_pixel * 8;
    self->info.bmiHeader.biCompression = BI_RGB;
    self->info.bmiHeader.biSize        = sizeof(self->info.bmiHeader);

    if (bit_buffer__resize(self, dims) == false) {
        return false;
    }

    memset(self->buffer, 0, self->dims.x * self->dims.y * self->bytes_per_pixel);

    return true;
}

void bit_buffer__destroy(struct bit_buffer* self) {
    if (self->buffer != NULL) {
        VirtualFree(self->buffer, 0, MEM_RELEASE);
    }
}

void bit_buffer__blit_to_window(struct bit_buffer* self, struct window* window, HDC window_device_context_handle) {
    StretchDIBits(
        window_device_context_handle,
        0, 0, window->dims.x, window->dims.y,
        0, 0, self->dims.x, self->dims.y,
        self->buffer, &self->info,
        DIB_RGB_COLORS, SRCCOPY
    );
}

bool bit_buffer__resize(struct bit_buffer* self, struct v2u32 new_dims) {
    if (self->buffer) {
        VirtualFree(self->buffer, 0, MEM_RELEASE);
    }

    self->dims                         = new_dims;
    self->info.bmiHeader.biWidth       = self->dims.x;
    self->info.bmiHeader.biHeight      = -self->dims.y;

    // TODO(david): think about the alignment for the allocation
    self->buffer = VirtualAlloc(NULL, self->dims.x * self->dims.y * self->bytes_per_pixel, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (self->buffer == NULL) {
        return false;
    }

    return true;
}
