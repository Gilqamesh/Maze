#include "renderer.h"

#include "world_grid.h"

#pragma comment(lib, "window.lib")

void renderer__create(struct renderer* self) {
    push_buffer__create(&self->push_buffer);
}

void renderer__destroy(struct renderer* self) {
    push_buffer__destroy(&self->push_buffer);
}

void renderer__push_rectangle(struct renderer* self, struct v2r32 center_p, struct v2r32 half_dims, enum color color) {
    push_buffer__push_rectangle(&self->push_buffer, center_p, half_dims, color);
}

void renderer__clear_push_buffer(struct renderer* self) {
    push_buffer__clear(&self->push_buffer);
}

void renderer__push_entities(
    struct renderer* self,
    struct window* window,
    struct world* world,
    struct world_position center_p,
    struct v2r32 half_dims
) {
    struct v2r32 world_grid_dims = world_grid__dims();
    struct v2r32 render_ratio = v2r32(
        (r32) window->dims.x / 2.0f / half_dims.x,
        (r32) window->dims.y / 2.0f / half_dims.y
    );

    struct v2r32 world_grid_half_dims = v2r32__scale_r32(world_grid_dims, 0.5f);
    world_grid_half_dims = v2r32__scale_v2r32(world_grid_half_dims, render_ratio);

    struct world_position start_world_p = world_position__from_relative_p(
        v2r32__scale_r32(half_dims, -1.0f),
        center_p
    );
    struct world_position end_world_p = world_position__from_relative_p(
        half_dims,
        center_p
    );

    struct v2r32 line_thickness = v2r32(
        render_ratio.x * 4.0f,
        render_ratio.y * 4.0f
    );

    for (i32 world_grid_y = start_world_p.global_p.y; world_grid_y <= end_world_p.global_p.y; ++world_grid_y) {
        for (i32 world_grid_x = start_world_p.global_p.x; world_grid_x <= end_world_p.global_p.x; ++world_grid_x) {
            struct v2i32 grid_p = v2i32(world_grid_x, world_grid_y);
            struct world_grid* grid = world__get_grid(world, grid_p);
            struct entity** entities = grid->entities;
            for (u32 entity_index = 0; entity_index < grid->entities_size; ++entity_index) {
                struct entity* entity = entities[entity_index];
                if (entity != NULL) {
                    struct v2r32 entity_p = world_position__to_relative_p(entity->center_p, center_p);
                    entity_p = v2r32__scale_v2r32(entity_p, render_ratio);
                    if (v2r32__is_in_half_dims(
                            entity_p,
                            v2r32((r32) window->dims.x / 2.0f, (r32) window->dims.y / 2.0f))
                        ) {
                        renderer__push_rectangle(
                            self,
                            entity_p,
                            line_thickness,
                            COLOR_BLUE
                        );
                    }
                }
            }
        }
    }
}

static void renderer__push_sim_region_rec(
    struct renderer* self,
    struct window* window,
    struct world_position center_p,
    struct v2r32 half_dims
) {
    struct v2r32 world_grid_dims = world_grid__dims();
    struct v2r32 render_ratio = v2r32(
        (r32) window->dims.x / 2.0f / half_dims.x,
        (r32) window->dims.y / 2.0f / half_dims.y
    );
    struct v2r32 world_grid_half_dims = v2r32__scale_r32(world_grid_dims, 0.5f);
    world_grid_half_dims = v2r32__scale_v2r32(world_grid_half_dims, render_ratio);

    struct world_position sim_region_world_p = world_position(v2i32(0, 0), v2r32(0.0f, 0.0f));
    struct v2r32 sim_region_center_p = world_position__to_relative_p(sim_region_world_p, center_p);
    sim_region_center_p = v2r32__scale_v2r32(sim_region_center_p, render_ratio);

    struct v2r32 window_half_dims = v2r32(
        (r32) window->dims.x / 2.0f,
        (r32) window->dims.y / 2.0f
    );
    window_half_dims = v2r32__scale_v2r32(window_half_dims, render_ratio);

    struct v2r32 top_left_p = v2r32__add_v2r32(sim_region_center_p, v2r32(
        -window_half_dims.x,
        -window_half_dims.y
    ));
    struct v2r32 top_right_p = v2r32__add_v2r32(sim_region_center_p, v2r32(
        window_half_dims.x,
        -window_half_dims.y
    ));
    struct v2r32 bottom_left_p = v2r32__add_v2r32(sim_region_center_p, v2r32(
        -window_half_dims.x,
        window_half_dims.y
    ));
    struct v2r32 bottom_right_p = v2r32__add_v2r32(sim_region_center_p, v2r32(
        window_half_dims.x,
        window_half_dims.y
    ));
    struct v2r32 line_thickness = v2r32(
        render_ratio.x * 2.0f,
        render_ratio.y * 2.0f
    );
    // top_left -> top_right
    renderer__push_rectangle(
        self,
        top_left_p,
        v2r32(top_right_p.x - top_left_p.x, line_thickness.y),
        COLOR_RED
    );
    // top_right -> bottom_right
    renderer__push_rectangle(
        self,
        top_right_p,
        v2r32(line_thickness.x, bottom_right_p.y - top_right_p.y),
        COLOR_RED
    );
    // top_left -> bottom_left
    renderer__push_rectangle(
        self,
        top_left_p,
        v2r32(line_thickness.x, bottom_left_p.y - top_left_p.y),
        COLOR_RED
    );
    // bottom_left -> bottom_right
    renderer__push_rectangle(
        self,
        bottom_left_p,
        v2r32(bottom_right_p.x - bottom_left_p.x, line_thickness.y),
        COLOR_RED
    );
}

static void renderer__push_world_grids(
    struct renderer* self,
    struct window* window,
    struct world* world,
    struct world_position center_p,
    struct v2r32 half_dims
) {
    struct v2r32 world_grid_dims = world_grid__dims();
    struct v2r32 render_ratio = v2r32(
        (r32) window->dims.x / 2.0f / half_dims.x,
        (r32) window->dims.y / 2.0f / half_dims.y
    );
    struct v2r32 world_grid_half_dims = v2r32__scale_r32(world_grid_dims, 0.5f);
    world_grid_half_dims = v2r32__scale_v2r32(world_grid_half_dims, render_ratio);

    struct world_position start_world_p = world_position__from_relative_p(
        v2r32__scale_r32(half_dims, -1.0f),
        center_p
    );
    struct world_position end_world_p = world_position__from_relative_p(
        half_dims,
        center_p
    );

    for (i32 world_grid_y = start_world_p.global_p.y; world_grid_y <= end_world_p.global_p.y; ++world_grid_y) {
        for (i32 world_grid_x = start_world_p.global_p.x; world_grid_x <= end_world_p.global_p.x; ++world_grid_x) {
            struct v2i32 grid_p = v2i32(world_grid_x, world_grid_y);
            struct world_grid* grid = world__get_grid(world, grid_p);
            struct world_position grid_world_p = world_position(
                grid->global_p,
                v2r32(0.0f, 0.0f)
            );
            struct v2r32 grid_center_p = world_position__to_relative_p(grid_world_p, center_p);
            grid_center_p = v2r32__scale_v2r32(grid_center_p, render_ratio);
            struct v2r32 top_left_p = v2r32__add_v2r32(grid_center_p, v2r32(
                -world_grid_half_dims.x,
                -world_grid_half_dims.y
            ));
            struct v2r32 top_right_p = v2r32__add_v2r32(grid_center_p, v2r32(
                world_grid_half_dims.x,
                -world_grid_half_dims.y
            ));
            struct v2r32 bottom_left_p = v2r32__add_v2r32(grid_center_p, v2r32(
                -world_grid_half_dims.x,
                world_grid_half_dims.y
            ));
            struct v2r32 bottom_right_p = v2r32__add_v2r32(grid_center_p, v2r32(
                world_grid_half_dims.x,
                world_grid_half_dims.y
            ));
            struct v2r32 line_thickness = v2r32(
                render_ratio.x * 2.0f,
                render_ratio.y * 2.0f
            );
            // top_left -> top_right
            renderer__push_rectangle(
                self,
                top_left_p,
                v2r32(top_right_p.x - top_left_p.x, line_thickness.y),
                COLOR_GREEN
            );
            // top_right -> bottom_right
            renderer__push_rectangle(
                self,
                top_right_p,
                v2r32(line_thickness.x, bottom_right_p.y - top_right_p.y),
                COLOR_GREEN
            );
            // top_left -> bottom_left
            renderer__push_rectangle(
                self,
                top_left_p,
                v2r32(line_thickness.x, bottom_left_p.y - top_left_p.y),
                COLOR_GREEN
            );
            // bottom_left -> bottom_right
            renderer__push_rectangle(
                self,
                bottom_left_p,
                v2r32(bottom_right_p.x - bottom_left_p.x, line_thickness.y),
                COLOR_GREEN
            );
        }
    }
}

void renderer__render(
    struct renderer* self,
    struct window* window,
    struct world* world,
    struct world_position center_p,
    struct v2r32 half_dims
) {
    // todo: render all pushed entities

    renderer__push_sim_region_rec(self, window, center_p, half_dims);
    renderer__push_world_grids(self, window, world, center_p, half_dims);
}
