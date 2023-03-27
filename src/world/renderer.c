#include "renderer.h"

#include "world_grid.h"

void renderer__create(struct renderer* self, struct window* window, struct world* world, struct world_position center_p, struct v2r32 half_dims) {
    self->window = window;
    self->world = world;
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
    window__draw_rectangle(self->window, v2r32(
        self->half_dims.x + position.x,
        self->half_dims.y + position.y
    ), dims, color);
}

void renderer__push_entities(struct renderer* self) {
    struct v2r32 world_grid_dims = world_grid__dims();

    struct world_position start_world_p = world_position__from_relative_p(
        v2r32(-self->half_dims.x, -self->half_dims.y),
        self->center_p
    );
    struct world_position end_world_p = world_position__from_relative_p(
        v2r32(self->half_dims.x, self->half_dims.y),
        self->center_p
    );

    for (i32 world_grid_y = start_world_p.global_p.y; world_grid_y <= end_world_p.global_p.y; ++world_grid_y) {
        for (i32 world_grid_x = start_world_p.global_p.x; world_grid_x <= end_world_p.global_p.x; ++world_grid_x) {
            struct v2i32 grid_p = v2i32(world_grid_x, world_grid_y);
            struct world_grid* grid = world__get_grid(self->world, grid_p);
            struct entity** entities = grid->entities;
            for (u32 entity_index = 0; entity_index < grid->entities_size; ++entity_index) {
                struct entity* entity = entities[entity_index];
                if (entity != NULL) {
                    struct v2r32 entity_p = world_position__to_relative_p(entity->p, self->center_p);
                    if (v2r32__is_in_half_dims(entity_p, self->half_dims)) {
                        renderer__push_rectangle(
                            self,
                            entity_p,
                            v2r32(5.0f, 5.0f),
                            COLOR_RED
                        );
                    }
                }
            }
        }
    }
}

static void renderer__draw_world_grids(struct renderer* self) {
    struct v2r32 world_grid_dims = world_grid__dims();
    struct v2r32 render_ratio = v2r32(
        2.0f * self->half_dims.x / (r32) self->window->dims.x,
        2.0f * self->half_dims.y / (r32) self->window->dims.y
    );
    struct v2r32 world_grid_half_dims = v2r32__scale_r32(world_grid_dims, 0.5f);
    world_grid_half_dims = v2r32__scale_v2r32(world_grid_half_dims, render_ratio);

    struct world_position start_world_p = world_position__from_relative_p(
        v2r32(-self->half_dims.x, -self->half_dims.y),
        self->center_p
    );
    struct world_position end_world_p = world_position__from_relative_p(
        v2r32(self->half_dims.x, self->half_dims.y),
        self->center_p
    );

    for (i32 world_grid_y = start_world_p.global_p.y; world_grid_y <= end_world_p.global_p.y; ++world_grid_y) {
        for (i32 world_grid_x = start_world_p.global_p.x; world_grid_x <= end_world_p.global_p.x; ++world_grid_x) {
            struct v2i32 grid_p = v2i32(world_grid_x, world_grid_y);
            struct world_grid* grid = world__get_grid(self->world, grid_p);
            struct world_position grid_world_p = world_position(
                grid->global_p,
                v2r32(0.0f, 0.0f)
            );
            struct v2r32 grid_relative_p = world_position__to_relative_p(grid_world_p, self->center_p);
            struct v2r32 top_left_p = v2r32__scale_v2r32(grid_relative_p, render_ratio);
            top_left_p = v2r32__add_v2r32(top_left_p, v2r32(
                -world_grid_half_dims.x,
                -world_grid_half_dims.y
            ));
            struct v2r32 top_right_p = v2r32__scale_v2r32(grid_relative_p, render_ratio);
            top_right_p = v2r32__add_v2r32(top_right_p, v2r32(
                world_grid_half_dims.x,
                -world_grid_half_dims.y
            ));
            struct v2r32 bottom_left_p = v2r32__scale_v2r32(grid_relative_p, render_ratio);
            bottom_left_p = v2r32__add_v2r32(bottom_left_p, v2r32(
                -world_grid_half_dims.x,
                world_grid_half_dims.y
            ));
            struct v2r32 bottom_right_p = v2r32__scale_v2r32(grid_relative_p, render_ratio);
            bottom_right_p = v2r32__add_v2r32(bottom_right_p, v2r32(
                world_grid_half_dims.x,
                world_grid_half_dims.y
            ));
            // top_left -> top_right
            renderer__push_rectangle(
                self,
                top_left_p,
                v2r32(top_right_p.x - top_left_p.x, 2.0f),
                COLOR_GREEN
            );
            // top_right -> bottom_right
            renderer__push_rectangle(
                self,
                top_right_p,
                v2r32(2.0f, bottom_right_p.y - top_right_p.y),
                COLOR_GREEN
            );
            // top_left -> bottom_left
            renderer__push_rectangle(
                self,
                top_left_p,
                v2r32(2.0f, bottom_left_p.y - top_left_p.y),
                COLOR_GREEN
            );
            // bottom_left -> bottom_right
            renderer__push_rectangle(
                self,
                bottom_left_p,
                v2r32(bottom_right_p.x - bottom_left_p.x, 2.0f),
                COLOR_GREEN
            );
        }
    }
}

void renderer__render(struct renderer* self) {
    // todo: render all pushed entities

    renderer__draw_world_grids(self);
}
