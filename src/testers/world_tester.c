#include "../window/window.h"
#include "../world/world.h"
#include "../world/sim_region.h"
#include "../random/random.h"

#pragma comment(lib, "world.lib")

void entity_processor_callback(struct entity** entities, struct renderer* renderer, u32 entities_size) {
    for (u32 entity_index = 0; entity_index < entities_size; ++entity_index) {
        struct entity* entity = entities[entity_index];
        if (entity != NULL) {
            if (entity__flag_is_set(entity, ENTITY_FLAGS__IS_IN_SIM_REGION)) {
                entity->sim_region_relative.relative_p.x += world__meters_to_pixels(0.001f);
                entity->sim_region_relative.relative_p.y += world__meters_to_pixels(0.001f);
            }
            if (renderer != NULL && entity__flag_is_set(entity, ENTITY_FLAGS__IS_IN_RENDER_REGION)) {
                renderer__push_rectangle(
                    renderer,
                    entity->sim_region_relative.relative_p,
                    v2r32(5.0f, 5.0f),
                    COLOR_RED
                );
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

    struct renderer renderer;
    renderer__create(
        &renderer,
        &window,
        world_position(
            v2i32(0, 0),
            v2r32(0.0f, 0.0f)
        ),
        v2r32((r32) window_dims.x / 2.0f, (r32) window_dims.y / 2.0f)
    );

    struct sim_region sim_region;

    struct entity* entities[128];
    u32 entities_fill = 0;

    while (window__does_exist(&window)) {
        window__clear_screen(&window, COLOR_WHITE);
        window__poll_inputs(&window);

        if (window__is_key_pressed(&window, KEY_RIGHT) &&
            entities_fill < sizeof(entities) / sizeof(entities[0])
        ) {
            struct v2r32 world_grid_dims = world_grid__dims();
            struct entity* entity = entity__create(
                world_position(
                    renderer.center_p.global_p,
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

        sim_region__routine(
            &sim_region,
            &world,
            &renderer,
            entity_processor_callback,
            renderer.center_p,
            renderer.half_dims
        );

        window__end_draw(&window);
    }
    window__destroy(&window);

    world__destroy(&world);

    window__deinit_module(&window);    
    console__deinit_module(&console);
}
