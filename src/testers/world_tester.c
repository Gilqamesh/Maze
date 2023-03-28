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
            if (entity->processor_callback_transient_values.is_in_sim_region) {
                if (entity__flag_is_set(entity, ENTITY_FLAGS__TEMPORARY_UPDATE_FLAG)) {
                    entity->processor_callback_transient_values.relative_p = v2r32__add_v2r32(
                        entity->processor_callback_transient_values.relative_p,
                        entity->dp
                    );
                }
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
        v2r32(
            (r32) window.dims.x / 2.0f,
            (r32) window.dims.y / 2.0f
        ),
        v2r32(
            (r32) window.dims.x / 2.0f,
            (r32) window.dims.y / 2.0f
        )
    );

    struct entity* entities[128];
    u32 entities_fill = 0;

    struct world_position sim_region_center_p = world_position(v2i32(0, 0), v2r32(0.0f, 0.0f));
    struct v2r32 sim_region_half_dims = v2r32(
        (r32) window.dims.x / 2.0f,
        (r32) window.dims.y / 2.0f
    );

    while (window__does_exist(&window)) {
        u64 start_time_stamp_counter = __rdtsc();
        window__clear_screen(&window, COLOR_WHITE);
        window__poll_inputs(&window);

        i32 wheel_delta = window__mouse_get_wheel_delta(&window);
        if (wheel_delta != 0) {
            struct v2r32 renderer_start_half_dims_relative_change = v2r32(
                (r32) wheel_delta * (camera_left.viewport_half_dims.x) / 2000.0f,
                (r32) wheel_delta * (camera_left.viewport_half_dims.y) / 2000.0f
            );
            console__log(&console, "%f %f\n", renderer_start_half_dims_relative_change.x, renderer_start_half_dims_relative_change.y);
            camera__update_viewport_half_dims_relative(&camera_left, renderer_start_half_dims_relative_change);
        }

        if (window__key_is_pressed(&window, KEY_RIGHT) &&
            entities_fill < sizeof(entities) / sizeof(entities[0])
        ) {
            struct v2r32 world_grid_dims = world_grid__dims();
            struct entity* entity = entity__create_absolute(
                sim_region_center_p,
                v2r32(1.0f, 1.0f),
                COLOR_BLUE
            );
            entity__flag_set(entity, ENTITY_FLAGS__TEMPORARY_UPDATE_FLAG);
            struct v2r32 random_entity_dp = v2r32(
                world__meters_to_pixels(random__r32_closed(&random, -0.01f, 0.01f)),
                world__meters_to_pixels(random__r32_closed(&random, -0.01f, 0.01f))
            );
            entity->dp = random_entity_dp;
            struct world_grid* grid = world__get_grid(&world, camera_left.viewport_center_p.global_p);
            world_grid__add_entity(grid, entity);
        }

        if (window__key_is_down(&window, MOUSE_LBUTTON)) {
            struct v2u32 mouse_p = window__mouse_get_position(&window);
            if (v2u32__is_less(mouse_p, window_dims)) {
                struct v2i32 mouse_dp = window__mouse_get_delta(&window);
                camera__update_viewport_p_relative(
                    &camera_left,
                    v2r32(
                        (r32) -mouse_dp.x,
                        (r32) -mouse_dp.y
                    )
                );
                console__log(&console, "%d %d, %f %f\n",
                    camera_left.viewport_center_p.global_p.x, camera_left.viewport_center_p.global_p.y,
                    camera_left.viewport_center_p.local_p.x, camera_left.viewport_center_p.local_p.y);
            }
        }

        sim_region__routine(
            &world,
            entity_processor_callback,
            NULL,
            sim_region_center_p,
            sim_region_half_dims
        );

        debug_push_sim_region_rec(&camera_left, sim_region_center_p, sim_region_half_dims);

        camera__render(&camera_left);

        window__end_draw(&window);
        u64 end_time_stamp_counter = __rdtsc();
        // console__log(&console, "Time stamp counter for frame: %u\n", end_time_stamp_counter - start_time_stamp_counter);
    }
    window__destroy(&window);

    world__destroy(&world);

    window__deinit_module(&window);
    console__deinit_module(&console);
}
