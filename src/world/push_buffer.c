#include "push_buffer.h"

#define PUSH_BUFFER_RECTANGLES_SIZE_START 16

struct push_buffer_rectangle push_buffer_rectangle__create(
    struct v3r32 center_p,
    struct v3r32 half_dims,
    enum color color
) {
    struct push_buffer_rectangle result = { 0 };

    result.center_p = center_p;
    result.half_dims = half_dims;
    result.color = color;

    return result;
}

void push_buffer__create(struct push_buffer* self) {
    self->rectangles_size = PUSH_BUFFER_RECTANGLES_SIZE_START;
    self->rectangles_fill = 0;
    self->rectangles = (struct push_buffer_rectangle*) malloc(self->rectangles_size * sizeof(*self->rectangles));
    if (self->rectangles == NULL) {
        ExitProcess(APP_ERROR_ALLOC_FAIL);
    }
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
    struct v3r32 center_p,
    struct v3r32 half_dims,
    enum color color
) {
    if (self->rectangles_fill == self->rectangles_size) {
        self->rectangles_size <<= 1;
        self->rectangles = (struct push_buffer_rectangle*) realloc(self->rectangles, self->rectangles_size * sizeof(*self->rectangles));
    }

    self->rectangles[self->rectangles_fill++] = push_buffer_rectangle__create(center_p, half_dims, color);
}

void push_buffer__sort_rectangles(struct push_buffer* self) {
    // todo: radix sort instead of bubble sort
    bool is_sorted = false;
    for (u32 outer_iter = 0; is_sorted == false && outer_iter < self->rectangles_fill; ++outer_iter) {
        for (i32 rec_index = 0; rec_index < self->rectangles_fill - outer_iter - 1; ++rec_index) {
            struct push_buffer_rectangle* rec_a = &self->rectangles[rec_index];
            struct push_buffer_rectangle* rec_b = &self->rectangles[rec_index + 1];
            if (rec_a->center_p.z > rec_b->center_p.z) {
                struct push_buffer_rectangle tmp = *rec_a;
                *rec_a = *rec_b;
                *rec_b = tmp;
            }
        }
    }
}
