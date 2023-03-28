#include "push_buffer.h"

#define PUSH_BUFFER_RECTANGLES_SIZE_START 16

struct push_buffer_rectangle push_buffer_rectangle__create(
    struct v2r32 center_p,
    struct v2r32 half_dims,
    enum color color
) {
    struct push_buffer_rectangle result = { 0 };

    result.center_p = center_p;
    result.half_dims = half_dims;
    result.color = color;
}

void push_buffer__create(struct push_buffer* self) {
    self->rectangles_size = PUSH_BUFFER_RECTANGLES_SIZE_START;
    self->rectangles_fill = 0;
}

void push_buffer__destroy(struct push_buffer* self) {
    free(self->rectangles);
}

void push_buffer__pop(struct push_buffer* self) {
    if (self->rectangles_fill > 0) {
        --self->rectangles_fill;
    }
}

void push_buffer__clear(struct push_buffer* self) {
    self->rectangles_fill = 0;
    // todo: think about memory management
}

void push_buffer__push_rectangle(
    struct push_buffer* self,
    struct v2r32 center_p,
    struct v2r32 half_dims,
    enum color color
) {
    if (self->rectangles_fill == self->rectangles_size) {
        self->rectangles_size <<= 1;
        self->rectangles = (struct push_buffer_rectangle*) realloc(self->rectangles, self->rectangles_size * sizeof(*self->rectangles));
    }

    self->rectangles[self->rectangles_fill++] = push_buffer_rectangle__create(center_p, half_dims, color);
}
