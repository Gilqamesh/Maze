#include "renderer.h"

#include "world_grid.h"

#include "../math/clamp.h"

void renderer__create(struct renderer* self) {
    push_buffer__create(&self->push_buffer);
}

void renderer__destroy(struct renderer* self) {
    push_buffer__destroy(&self->push_buffer);
}

void renderer__push_rectangle_centered(struct renderer* self, struct v2r32 center_p, struct v2r32 half_dims, enum color color) {
    push_buffer__push_rectangle(&self->push_buffer, center_p, half_dims, color);
}

void renderer__push_rectangle_top_left(struct renderer* self, struct v2r32 top_left_p, struct v2r32 dims, enum color color) {
    struct v2r32 half_dims = v2r32__scale_r32(dims, 0.5f);
    struct v2r32 center_p = v2r32__add_v2r32(top_left_p, half_dims);
    renderer__push_rectangle_centered(self, center_p, half_dims, color);
}

void renderer__clear_push_buffer(struct renderer* self) {
    push_buffer__clear(&self->push_buffer);
}

void renderer__render(
    struct renderer* self,
    struct window* window,
    struct world* world,
    struct world_position viewport_p,
    struct v2r32 viewport_half_dims,
    struct v2r32 window_client_top_left_p_normalized,
    struct v2r32 window_client_bot_right_p_normalized
) {
    window_client_top_left_p_normalized = v2r32__scale_v2r32(
        window_client_top_left_p_normalized,
        v2r32((r32) window->dims.x, (r32) window->dims.y)
    );
    window_client_bot_right_p_normalized = v2r32__scale_v2r32(
        window_client_bot_right_p_normalized,
        v2r32((r32) window->dims.x, (r32) window->dims.y)
    );
    struct v2r32 window_client_half_dims = v2r32(
        (window_client_bot_right_p_normalized.x - window_client_top_left_p_normalized.x) / 2.0f,
        (window_client_bot_right_p_normalized.y - window_client_top_left_p_normalized.y) / 2.0f
    );
    struct v2r32 window_client_center_p = v2r32__add_v2r32(
        window_client_top_left_p_normalized,
        window_client_half_dims
    );

    // note: perspective divide
    struct v2r32 renderer_perspective_divide = v2r32(
        window_client_half_dims.x / viewport_half_dims.x,
        window_client_half_dims.y / viewport_half_dims.y
    );

    // note: viewport transport
    struct v2r32 renderer_viewport_transport = window_client_center_p;

    // note: clipping values
    struct v2r32 render_clip_dims = v2r32__scale_r32(window_client_half_dims, 2.0f);
    struct v2r32 render_clip_top_left_p = v2r32(
        window_client_center_p.x - window_client_half_dims.x,
        window_client_center_p.y - window_client_half_dims.y
    );
    struct v2r32 render_clip_bottom_right_p = v2r32(
        window_client_center_p.x + window_client_half_dims.x,
        window_client_center_p.y + window_client_half_dims.y
    );

    for (u32 push_buffer_index = 0; push_buffer_index < self->push_buffer.rectangles_fill; ++push_buffer_index) {
        struct push_buffer_rectangle* rectangle = &self->push_buffer.rectangles[push_buffer_index];
        struct v2r32 rectangle_top_left_p = v2r32__add_v2r32(
            rectangle->center_p,
            v2r32__scale_r32(rectangle->half_dims, -1.0f)
        );

        // note: perspective divide
        struct v2r32 rectangle_dims = v2r32__scale_r32(rectangle->half_dims, 2.0f);
        rectangle_dims = v2r32__scale_v2r32(rectangle_dims, renderer_perspective_divide);

        rectangle_top_left_p = v2r32__scale_v2r32(rectangle_top_left_p, renderer_perspective_divide);

        // note: viewport transport
        rectangle_top_left_p = v2r32__add_v2r32(rectangle_top_left_p, renderer_viewport_transport);

        // note: clipping
        struct v2r32 rectangle_bottom_right = v2r32__add_v2r32(rectangle_top_left_p, rectangle_dims);
        rectangle_top_left_p = clamp__v2r32(
            render_clip_top_left_p,
            rectangle_top_left_p,
            render_clip_bottom_right_p
        );
        rectangle_bottom_right = clamp__v2r32(
            render_clip_top_left_p,
            rectangle_bottom_right,
            render_clip_bottom_right_p
        );
        rectangle_dims = v2r32__sub_v2r32(rectangle_bottom_right, rectangle_top_left_p);

        /*
         * per-sample processing
         */

        // scissor test
        if (rectangle_dims.x == 0.0f || rectangle_dims.y == 0.0f) {
            continue ;
        }

        if (rectangle->color == COLOR_RED) {
            i32 debug = 0;
            ++debug;
        }

        // anti-aliasing

        window__draw_rectangle_aa(
            window,
            rectangle_top_left_p,
            rectangle_dims,
            rectangle->color
        );
    }
}
