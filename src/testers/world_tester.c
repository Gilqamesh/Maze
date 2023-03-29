#include "../window/window.h"
#include "../world/world.h"
#include "../world/sim_region.h"
#include "../world/camera.h"
#include "../random/random.h"

void entity_processor_callback(void* context, struct entity** entities, u32 entities_size) {
    (void) context;

    for (u32 entity_index = 0; entity_index < entities_size; ++entity_index) {
        struct entity* entity = entities[entity_index];
        if (entity != NULL) {
            if (entity__flag_is_set(entity, ENTITY_FLAGS_IS_IN_SIM_REGION)
            ) {
                entity__flag_set(entity, ENTITY_FLAGS_SIMULATED_ALREADY);

                entity->processor_callback_transient_values.relative_p = v2r32__add_v2r32(
                    entity->processor_callback_transient_values.relative_p,
                    entity->dp
                );
            }
        }
    }
}

void debug_push_sim_region_rec(struct camera* camera, struct world_position sim_region_center_p, struct v2r32 sim_region_half_dims) {
    struct v2r32 sim_region_relative_center_p = world_position__to_relative_p(sim_region_center_p, camera->viewport_center_p);

    struct v2r32 top_left_p = v2r32__add_v2r32(sim_region_relative_center_p, v2r32(
        -sim_region_half_dims.x,
        -sim_region_half_dims.y
    ));
    struct v2r32 top_right_p = v2r32__add_v2r32(sim_region_relative_center_p, v2r32(
        sim_region_half_dims.x,
        -sim_region_half_dims.y
    ));
    struct v2r32 bottom_left_p = v2r32__add_v2r32(sim_region_relative_center_p, v2r32(
        -sim_region_half_dims.x,
        sim_region_half_dims.y
    ));
    struct v2r32 bottom_right_p = v2r32__add_v2r32(sim_region_relative_center_p, v2r32(
        sim_region_half_dims.x,
        sim_region_half_dims.y
    ));
    struct v2r32 rec_thickness = v2r32(2.0f, 2.0f);

    // top_left -> top_right
    renderer__push_rectangle_top_left(
        &camera->renderer,
        top_left_p,
        v2r32(top_right_p.x - top_left_p.x, rec_thickness.y),
        COLOR_RED
    );
    // top_right -> bottom_right
    renderer__push_rectangle_top_left(
        &camera->renderer,
        top_right_p,
        v2r32(rec_thickness.x, bottom_right_p.y - top_right_p.y),
        COLOR_RED
    );
    // top_left -> bottom_left
    renderer__push_rectangle_top_left(
        &camera->renderer,
        top_left_p,
        v2r32(rec_thickness.x, bottom_left_p.y - top_left_p.y),
        COLOR_RED
    );
    // bottom_left -> bottom_right
    renderer__push_rectangle_top_left(
        &camera->renderer,
        bottom_left_p,
        v2r32(bottom_right_p.x - bottom_left_p.x, rec_thickness.y),
        COLOR_RED
    );
}

void debug_push_world_grids(struct camera* camera) {
    struct v2r32 world_grid_half_dims = v2r32__scale_r32(world_grid__dims(), 0.5f);

    struct world_position start_world_p = world_position__from_relative_p(
        v2r32__scale_r32(camera->viewport_half_dims, -1.0f),
        camera->viewport_center_p
    );
    struct world_position end_world_p = world_position__from_relative_p(
        camera->viewport_half_dims,
        camera->viewport_center_p
    );

    for (i32 world_grid_y = start_world_p.global_p.y; world_grid_y <= end_world_p.global_p.y; ++world_grid_y) {
        for (i32 world_grid_x = start_world_p.global_p.x; world_grid_x <= end_world_p.global_p.x; ++world_grid_x) {
            struct v2i32 grid_p = v2i32(world_grid_x, world_grid_y);
            struct world_grid* grid = world__get_grid(camera->world, grid_p);
            struct world_position grid_world_p = world_position(
                grid->global_p,
                v2r32(0.0f, 0.0f)
            );
            struct v2r32 grid_center_p = world_position__to_relative_p(grid_world_p, camera->viewport_center_p);
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
            // top_left -> top_right
            renderer__push_rectangle_centered(
                &camera->renderer,
                top_left_p,
                v2r32(top_right_p.x - top_left_p.x, 2.0f),
                COLOR_GREEN
            );
            // top_right -> bottom_right
            renderer__push_rectangle_centered(
                &camera->renderer,
                top_right_p,
                v2r32(2.0f, bottom_right_p.y - top_right_p.y),
                COLOR_GREEN
            );
            // top_left -> bottom_left
            renderer__push_rectangle_centered(
                &camera->renderer,
                top_left_p,
                v2r32(2.0f, bottom_left_p.y - top_left_p.y),
                COLOR_GREEN
            );
            // bottom_left -> bottom_right
            renderer__push_rectangle_centered(
                &camera->renderer,
                bottom_left_p,
                v2r32(bottom_right_p.x - bottom_left_p.x, 2.0f),
                COLOR_GREEN
            );
        }
    }
}

int WinMain(HINSTANCE app_handle, HINSTANCE prev_instance, LPSTR cmd_line, int show_cmd) {
    struct console console;

    if (!console__init_module(&console, 512)) {
        // ExitProcess(EXIT_FAILURE);
    }

    struct random random;
    random__init(&random, 42);

    struct world world;
    world__create(&world);

    struct window window;
    window__init_module(&window, &console);
    
    struct v2u32 window_p = v2u32(100, 100);
    struct v2u32 window_dims = v2u32(800, 600);
    window__create(&window, app_handle, "World Tester Window", window_p, window_dims);

    struct camera camera_left;
    struct v2r32 camera_left_window_client_top_left_p_normalized = v2r32(0.1f, 0.1f);
    struct v2r32 camera_left_window_client_bot_right_p_normalized = v2r32(0.4f, 0.4f);
    camera__create(
        &camera_left,
        &window,
        &world,
        world_position(
            v2i32(0, 0),
            v2r32(0.0f, 0.0f)
        ),
        v2r32(
            (r32) window.dims.x / 2.0f,
            (r32) window.dims.y / 2.0f
        ),
        camera_left_window_client_top_left_p_normalized,
        camera_left_window_client_bot_right_p_normalized
    );

    struct camera camera_right;
    struct v2r32 camera_right_window_client_top_left_p_normalized = v2r32(0.6f, 0.1f);
    struct v2r32 camera_right_window_client_bot_right_p_normalized = v2r32(0.9f, 0.9f);
    camera__create(
        &camera_right,
        &window,
        &world,
        world_position(
            v2i32(3, 3),
            v2r32(0.0f, 0.0f)
        ),
        v2r32(
            2.0f * (r32) window.dims.x,
            2.0f * (r32) window.dims.y
        ),
        camera_right_window_client_top_left_p_normalized,
        camera_right_window_client_bot_right_p_normalized
    );

    struct entity* hero_entity = entity__create_absolute(
        world_position(v2i32(0, 0), v2r32(0.0f, 0.0f)),
        v2r32(
            world__meters_to_pixels(1.5f),
            world__meters_to_pixels(0.3f)
        ),
        COLOR_CYAN
    );
    struct world_grid* _grid = world__get_grid(&world, hero_entity->center_p.global_p);
    world_grid__add_entity(_grid, hero_entity);

    struct camera camera_entity;
    struct v2r32 camera_entity_window_client_top_left_p_normalized = v2r32(0.1f, 0.6f);
    struct v2r32 camera_entity_window_client_bot_right_p_normalized = v2r32(0.4f, 0.9f);
    camera__create(
        &camera_entity,
        &window,
        &world,
        hero_entity->center_p,
        v2r32(
            2.0f * (r32) window.dims.x,
            2.0f * (r32) window.dims.y
        ),
        camera_entity_window_client_top_left_p_normalized,
        camera_entity_window_client_bot_right_p_normalized
    );

    struct world_position sim_region_center_p = world_position(v2i32(0, 0), v2r32(0.0f, 0.0f));
    struct v2r32 sim_region_half_dims = v2r32(
        (r32) window.dims.x / 2.0f,
        (r32) window.dims.y / 2.0f
    );

    while (window__does_exist(&window)) {
        u64 start_time_stamp_counter = __rdtsc();
        window__clear_screen(&window, COLOR_WHITE);
        window__poll_inputs(&window);

        camera__update_viewport_p_absolute(&camera_entity, hero_entity->center_p);
        sim_region_center_p = hero_entity->center_p;
        hero_entity->dp.x = 0.0f;
        hero_entity->dp.y = 0.0f;
        if (window__key_is_down(&window, KEY_RIGHT)) {
            hero_entity->dp.x = world__meters_to_pixels(0.001f);
        }
        if (window__key_is_down(&window, KEY_LEFT)) {
            hero_entity->dp.x = world__meters_to_pixels(-0.001f);
        }
        if (window__key_is_down(&window, KEY_UP)) {
            hero_entity->dp.y = world__meters_to_pixels(-0.001f);
        }
        if (window__key_is_down(&window, KEY_DOWN)) {
            hero_entity->dp.y = world__meters_to_pixels(0.001f);
        }

        i32 wheel_delta = window__mouse_get_wheel_delta(&window);
        if (wheel_delta != 0) {
            struct v2u32 mouse_p = window__mouse_get_position(&window);
            if (camera__is_p_in_window_client_area(&camera_left, v2r32((r32) mouse_p.x, (r32) mouse_p.y))) {
                struct v2r32 renderer_start_half_dims_relative_change = v2r32(
                    (r32) wheel_delta * (camera_left.viewport_half_dims.x) / 2000.0f,
                    (r32) wheel_delta * (camera_left.viewport_half_dims.y) / 2000.0f
                );
                // console__log(&console, "%f %f\n", renderer_start_half_dims_relative_change.x, renderer_start_half_dims_relative_change.y);
                camera__update_viewport_half_dims_relative(&camera_left, renderer_start_half_dims_relative_change);
            }
            if (camera__is_p_in_window_client_area(&camera_right, v2r32((r32) mouse_p.x, (r32) mouse_p.y))) {
                struct v2r32 renderer_start_half_dims_relative_change = v2r32(
                    (r32) wheel_delta * (camera_right.viewport_half_dims.x) / 2000.0f,
                    (r32) wheel_delta * (camera_right.viewport_half_dims.y) / 2000.0f
                );
                // console__log(&console, "%f %f\n", renderer_start_half_dims_relative_change.x, renderer_start_half_dims_relative_change.y);
                camera__update_viewport_half_dims_relative(&camera_right, renderer_start_half_dims_relative_change);
            }
            if (camera__is_p_in_window_client_area(&camera_entity, v2r32((r32) mouse_p.x, (r32) mouse_p.y))) {
                struct v2r32 renderer_start_half_dims_relative_change = v2r32(
                    (r32) wheel_delta * (camera_entity.viewport_half_dims.x) / 2000.0f,
                    (r32) wheel_delta * (camera_entity.viewport_half_dims.y) / 2000.0f
                );
                // console__log(&console, "%f %f\n", renderer_start_half_dims_relative_change.x, renderer_start_half_dims_relative_change.y);
                camera__update_viewport_half_dims_relative(&camera_entity, renderer_start_half_dims_relative_change);
            }
        }

        if (window__key_is_down(&window, KEY_SPACE)) {
            struct v2r32 world_grid_dims = world_grid__dims();
            struct entity* entity = entity__create_relative(
                v2r32(0.0f, 0.0f),
                hero_entity->center_p,
                v2r32(
                    world__meters_to_pixels(0.1f),
                    world__meters_to_pixels(0.1f)
                ),
                COLOR_BLUE
            );
            entity->dp = v2r32(
                world__meters_to_pixels(random__r32_closed(&random, -1.0f, 1.0f)),
                world__meters_to_pixels(random__r32_closed(&random, -1.0f, 1.0f))
            );
            struct world_grid* grid = world__get_grid(&world, hero_entity->center_p.global_p);
            world_grid__add_entity(grid, entity);
        }

        if (window__key_is_down(&window, MOUSE_LBUTTON)) {
            struct v2u32 mouse_p = window__mouse_get_position(&window);
            if (camera__is_p_in_window_client_area(&camera_left, v2r32((r32) mouse_p.x, (r32) mouse_p.y))) {
                struct v2i32 mouse_dp = window__mouse_get_delta(&window);
                camera__update_viewport_p_relative(
                    &camera_left,
                    v2r32(
                        3.0f * (r32) -mouse_dp.x,
                        3.0f * (r32) -mouse_dp.y
                    )
                );
            }
            if (camera__is_p_in_window_client_area(&camera_right, v2r32((r32) mouse_p.x, (r32) mouse_p.y))) {
                struct v2i32 mouse_dp = window__mouse_get_delta(&window);
                camera__update_viewport_p_relative(
                    &camera_right,
                    v2r32(
                        3.0f * (r32) -mouse_dp.x,
                        3.0f * (r32) -mouse_dp.y
                    )
                );
            }
        }

        sim_region__routine(
            &world,
            entity_processor_callback,
            NULL,
            sim_region_center_p,
            sim_region_half_dims
        );

        debug_push_world_grids(&camera_left);
        debug_push_world_grids(&camera_right);
        debug_push_world_grids(&camera_entity);
        debug_push_sim_region_rec(&camera_left, sim_region_center_p, sim_region_half_dims);
        debug_push_sim_region_rec(&camera_right, sim_region_center_p, sim_region_half_dims);
        debug_push_sim_region_rec(&camera_entity, sim_region_center_p, sim_region_half_dims);

        camera__render(&camera_right);
        camera__render(&camera_left);
        camera__render(&camera_entity);

        window__end_draw(&window);
        u64 end_time_stamp_counter = __rdtsc();
        console__log(&console, "Time stamp counter for frame: %u\n", end_time_stamp_counter - start_time_stamp_counter);
    }

    camera__destroy(&camera_right);
    camera__destroy(&camera_left);

    window__destroy(&window);

    world__destroy(&world);

    window__deinit_module(&window);
    console__deinit_module(&console);
}
