#include "button.h"

void button__create(struct button* self, struct window* window, struct v2r32 p, struct v2r32 dims, enum color color_passive, enum color color_hovered, enum color color_active) {
    self->window               = window;
    self->p                    = p;
    self->dims                 = dims;
    self->original_window_dims = window->dims;
    self->color_passive        = color_passive;
    self->color_hovered        = color_hovered;
    self->color_active         = color_active;
}

void button__destroy(struct button* self) {
}

void button__update(struct button* self) {
    struct v2u32 mp           = window__mouse_get_position(self->window);
    struct v2r32 window_ratio = v2r32((r32) self->window->dims.x / (r32) self->original_window_dims.x, (r32) self->window->dims.y / (r32) self->original_window_dims.y);
    struct v2r32 cur_p        = v2r32(self->p.x * window_ratio.x, self->p.y * window_ratio.y);
    struct v2r32 cur_dims     = v2r32(self->dims.x * window_ratio.x, self->dims.y * window_ratio.y);
    if (
        mp.x >= cur_p.x &&
        mp.x <= cur_p.x + cur_dims.x &&
        mp.y >= cur_p.y &&
        mp.y <= cur_p.y + cur_dims.y
    ) {
        self->is_hovered = true;
        if (window__is_key_pressed(self->window, MOUSE_LBUTTON)) {
            self->is_pressed = true;
        } else {
            self->is_pressed = false;
        }
    } else {
        self->is_pressed = false;
        self->is_hovered = false;
    }
}

enum color button__get_color(struct button* self) {
    if (self->is_pressed) {
        return self->color_active;
    }

    if (self->is_hovered) {
        return self->color_hovered;
    }

    return self->color_passive;
}

bool button__is_pressed(struct button* self) {
    return self->is_pressed;
}

void button__render(struct button* self) {
    struct v2r32 window_ratio = v2r32((r32) self->window->dims.x / (r32) self->original_window_dims.x, (r32) self->window->dims.y / (r32) self->original_window_dims.y);
    struct v2r32 cur_p        = v2r32(self->p.x * window_ratio.x, self->p.y * window_ratio.y);
    struct v2r32 cur_dims     = v2r32(self->dims.x * window_ratio.x, self->dims.y * window_ratio.y);
    window__draw_rectangle(self->window, cur_p, cur_dims, button__get_color(self));
}
