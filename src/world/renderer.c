#include "renderer.h"

void renderer__create(struct renderer* self, struct window* window, struct world_position center_p, struct v2r32 half_dims) {
    self->window = window;
    self->center_p = center_p;
    self->half_dims = half_dims;
}

void renrerer__destroy(struct renderer* self) {
}

#pragma comment(lib, "window.lib")

void renderer__push_pixel(struct renderer* self, struct v2r32 pixel, enum color color) {
    window__draw_pixel(self->window, v2u32(
        (u32) (self->half_dims.x + pixel.x),
        (u32) (self->half_dims.y + pixel.y)
    ), color);
}

void renderer__push_rectangle(struct renderer* self, struct v2r32 position, struct v2r32 dims, enum color color) {
    window__draw_rectangle(self->window, position, dims, color);
}
