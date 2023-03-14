#include "ui_group.h"

void ui_group__create(struct ui_group* self, struct window* window, struct v2r32 p, struct v2r32 dims, enum color color_passive, enum color color_hovered) {
    self->window               = window;
    self->buttons              = NULL;
    self->boxes                = NULL;
    self->p                    = p;
    self->dims                 = dims;
    self->original_window_dims = window->dims;
    self->color_passive        = color_passive;
    self->color_hovered        = color_hovered;
    self->is_hovered           = false;
}

void ui_group__destroy(struct ui_group* self) {
    while (self->buttons) {
        struct button* next = self->buttons->next;
        button__destroy(self->buttons);
        self->buttons = next;
    }
    while (self->boxes) {
        struct box* next = self->boxes->next;
        box__destroy(self->boxes);
        self->boxes = next;
    }
}

void ui_group__push_button(struct ui_group* self, struct button* button) {
    button->next = self->buttons;
    self->buttons = button;
}

void ui_group__push_box(struct ui_group* self, struct box* box) {
    box->next = self->boxes;
    self->boxes = box;
}

void ui_group__update_and_render(struct ui_group* self) {
    struct v2u32 mp               = window__mouse_get_position(self->window);
    struct v2r32 ui_group_scale   = v2r32((r32) self->window->dims.x / (r32) self->original_window_dims.x, (r32) self->window->dims.y / (r32) self->original_window_dims.y);
    struct v2r32 cur_p            = v2r32(self->p.x * ui_group_scale.x, self->p.y * ui_group_scale.y);
    struct v2r32 cur_dims         = v2r32(self->dims.x * ui_group_scale.x, self->dims.y * ui_group_scale.y);
    bool is_mouse_lbutton_pressed = window__is_key_pressed(self->window, MOUSE_LBUTTON);
    if (
        mp.x >= cur_p.x &&
        mp.x <= cur_p.x + cur_dims.x &&
        mp.y >= cur_p.y &&
        mp.y <= cur_p.y + cur_dims.y
    ) {
        self->is_hovered = true;
    } else {
        self->is_hovered = false;
    }

    enum color ui_group_color = ui_group__get_color(self);
    window__draw_rectangle(self->window, cur_p, cur_dims, ui_group_color);

    struct button* cur_button = self->buttons;
    while (cur_button) {
        button__update_and_render(cur_button, self->window, mp, cur_p, cur_dims, ui_group_scale, is_mouse_lbutton_pressed);
        cur_button = cur_button->next;
    }

    struct box* cur_box = self->boxes;
    while (cur_box) {
        box__update_and_render(cur_box, self->window, mp, cur_p, cur_dims, ui_group_scale);
        cur_box = cur_box->next;
    }
}

enum color ui_group__get_color(struct ui_group* self) {
    if (self->is_hovered) {
        return self->color_hovered;
    }

    return self->color_passive;
}
