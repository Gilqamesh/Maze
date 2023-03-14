#include "button.h"
#include "../math/clamp.h"

void button__create(struct button* self, struct v2r32 p, struct v2r32 dims, enum color color_passive, enum color color_hovered, enum color color_active) {
    self->next                 = NULL;
    self->p                    = p;
    self->dims                 = dims;
    self->color_passive        = color_passive;
    self->color_hovered        = color_hovered;
    self->color_active         = color_active;
    self->is_pressed           = false;
    self->is_hovered           = false;
}

void button__destroy(struct button* self) {
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

void button__update_and_render(struct button* self, struct window* window, struct v2u32 mp, struct v2r32 ui_group_offset, struct v2r32 ui_group_dims, struct v2r32 ui_group_scale, bool is_mouse_lbutton_pressed) {
    struct v2r32 cur_p    = v2r32(self->p.x * ui_group_scale.x + ui_group_offset.x, self->p.y * ui_group_scale.y + ui_group_offset.y);
    cur_p = clamp__v2r32(ui_group_offset, cur_p, v2r32(ui_group_offset.x + ui_group_dims.x, ui_group_offset.y + ui_group_dims.y));
    
    struct v2r32 cur_dims = v2r32(self->dims.x * ui_group_scale.x, self->dims.y * ui_group_scale.y);
    cur_dims = clamp__v2r32(cur_dims, cur_dims, v2r32(ui_group_offset.x + ui_group_dims.x - cur_p.x, ui_group_offset.y + ui_group_dims.y - cur_p.y));

    if (
        mp.x >= cur_p.x &&
        mp.x <= cur_p.x + cur_dims.x &&
        mp.y >= cur_p.y &&
        mp.y <= cur_p.y + cur_dims.y
    ) {
        self->is_hovered = true;
        if (is_mouse_lbutton_pressed) {
            self->is_pressed = true;
        } else {
            self->is_pressed = false;
        }
    } else {
        self->is_pressed = false;
        self->is_hovered = false;
    }
    enum color button_color = button__get_color(self);
    window__draw_rectangle(window, cur_p, cur_dims, button_color);
}
