#include "box.h"
#include "../math/clamp.h"

void box__create(struct box* self, struct v2r32 p, struct v2r32 dims, enum color color) {
    self->next       = NULL;
    self->p          = p;
    self->dims       = dims;
    self->color      = color;
}

void box__destroy(struct box* self) {
}

enum color box__get_color(struct box* self) {
    return self->color;
}

void box__update_and_render(
    struct box* self,
    struct window* window,
    struct v2i32 mp,
    struct v2r32 ui_group_offset,
    struct v2r32 ui_group_dims,
    struct v2r32 ui_group_scale
) {
    struct v2r32 cur_p    = v2r32(self->p.x * ui_group_scale.x + ui_group_offset.x, self->p.y * ui_group_scale.y + ui_group_offset.y);
    cur_p = clamp__v2r32(ui_group_offset, cur_p, v2r32(ui_group_offset.x + ui_group_dims.x, ui_group_offset.y + ui_group_dims.y));
    
    struct v2r32 cur_dims = v2r32(self->dims.x * ui_group_scale.x, self->dims.y * ui_group_scale.y);
    cur_dims = clamp__v2r32(cur_dims, cur_dims, v2r32(ui_group_offset.x + ui_group_dims.x - cur_p.x, ui_group_offset.y + ui_group_dims.y - cur_p.y));

    enum color button_color = button__get_color(self);
    window__draw_rectangle(window, cur_p, cur_dims, button_color);
}
