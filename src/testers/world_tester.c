#include "../window/window.h"
#include "../world/world.h"
#include "../world/sim_region.h"
#include "../random/random.h"

#pragma comment(lib, "world.lib")

void entity_processor_callback(struct entity** entities, u32 entities_size) {
    for (u32 entity_index = 0; entity_index < entities_size; ++entity_index) {
        struct entity* entity = entities[entity_index];
        if (entity != NULL) {
            if (entity__flag_is_set(entity, ENTITY_FLAGS__IS_IN_SIM_REGION)) {
                entity->sim_region_relative.relative_p.x += world__meters_to_pixels(0.01f);
                entity->sim_region_relative.relative_p.y += world__meters_to_pixels(0.01f);
            }
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

    struct v2r32 renderer_start_half_dims = v2r32((r32) window_dims.x / 2.0f, (r32) window_dims.y / 2.0f);
    struct renderer renderer;
    renderer__create(
        &renderer,
        &window,
        &world,
        world_position(
            v2i32(0, 0),
            v2r32(0.0f, 0.0f)
        ),
        renderer_start_half_dims
    );

    struct entity* entities[128];
    u32 entities_fill = 0;

    while (window__does_exist(&window)) {
        u64 start_time_stamp_counter = __rdtsc();
        window__clear_screen(&window, COLOR_WHITE);
        window__poll_inputs(&window);

        i32 wheel_delta = window__mouse_get_wheel_delta(&window);
        if (wheel_delta != 0) {
            struct v2r32 renderer_start_half_dims_relative_change = v2r32(
                (r32) wheel_delta * (renderer_start_half_dims.x) / 2000.0f,
                (r32) wheel_delta * (renderer_start_half_dims.y) / 2000.0f
            );
            console__log(&console, "%f %f\n", renderer_start_half_dims_relative_change.x, renderer_start_half_dims_relative_change.y);
            struct v2r32 renderer_new_half_dims = v2r32(
                renderer.half_dims.x + renderer_start_half_dims_relative_change.x,
                renderer.half_dims.y + renderer_start_half_dims_relative_change.y
            );
            renderer.half_dims = renderer_new_half_dims;
        }

        if (window__key_is_pressed(&window, KEY_RIGHT) &&
            entities_fill < sizeof(entities) / sizeof(entities[0])
        ) {
            struct v2r32 world_grid_dims = world_grid__dims();
            struct entity* entity = entity__create(
                world_position(
                    v2i32(0, 0),
                    v2r32(
                        random__r32_closed(&random, -world_grid_dims.x / 2.0f, world_grid_dims.x / 2.0f),
                        random__r32_closed(&random, -world_grid_dims.y / 2.0f, world_grid_dims.y / 2.0f)
                    )
                ),
                v2u32(2, 2)
            );
            struct world_grid* grid = world__get_grid(&world, renderer.center_p.global_p);
            world_grid__add_entity(grid, entity);
        }

        if (window__key_is_down(&window, MOUSE_LBUTTON)) {
            struct v2u32 mouse_p = window__mouse_get_position(&window);
            if (v2u32__is_less(mouse_p, window_dims)) {
                struct v2i32 mouse_dp = window__mouse_get_delta(&window);
                renderer.center_p = world_position__add_relative_p(
                    renderer.center_p,
                    v2r32(
                        (r32) -mouse_dp.x,
                        (r32) -mouse_dp.y
                    )
                );
                // console__log(&console, "%d %d, %f %f\n",
                //     renderer.center_p.global_p.x, renderer.center_p.global_p.y,
                //     renderer.center_p.local_p.x, renderer.center_p.local_p.y);
            }
        }

        sim_region__routine(
            &world,
            entity_processor_callback,
            world_position(v2i32(0, 0), v2r32(0.0f, 0.0f)),
            v2r32(
                (r32) window.dims.x / 2.0f,
                (r32) window.dims.y / 2.0f
            )
        );

        renderer__push_entities(&renderer);

        renderer__render(&renderer);

        window__end_draw(&window);
        u64 end_time_stamp_counter = __rdtsc();
        // console__log(&console, "Time stamp counter for frame: %u\n", end_time_stamp_counter - start_time_stamp_counter);
    }
    window__destroy(&window);

    world__destroy(&world);

    window__deinit_module(&window);
    console__deinit_module(&console);
}
