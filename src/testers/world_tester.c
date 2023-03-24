#include "../window/window.h"
#include "../world/world.h"
#include "../world/sim_region.h"

#pragma comment(lib, "world.lib")

void entity_processor_callback(struct entity** entities, u32 entities_size, struct world_position relative_p) {
    for (u32 entity_index = 0; entity_index < entities_size; ++entity_index) {
        struct entity* entity = entities[entity_index];
        if (entity != NULL) {
            struct v2r32 entity_p = world_position__to_relative_p(entity->p, relative_p);
            entity_p.x += world__meters_to_pixels(0.0001f);
            entity_p.y += world__meters_to_pixels(0.0001f);
            entity->p = world_position__from_relative_p(entity_p, relative_p);
        }
    }
}

int WinMain(HINSTANCE app_handle, HINSTANCE prev_instance, LPSTR cmd_line, int show_cmd) {
    struct console console;

    if (!console__init_module(&console, 512)) {
        // ExitProcess(EXIT_FAILURE);
    }

    struct world world;
    world__create(&world);

    struct window window;
    window__init_module(&window, &console);
    
    struct v2u32 window_p = v2u32(100, 100);
    struct v2u32 window_dims = v2u32(800, 600);
    window__create(&window, app_handle, "World Tester Window", window_p, window_dims);

    while (window__does_exist(&window)) {
        window__clear_screen(&window, COLOR_WHITE);
        window__poll_inputs(&window);

        window__end_draw(&window);
    }
    window__destroy(&window);

    world__destroy(&world);

    window__deinit_module(&window);    
    console__deinit_module(&console);
}
