#include "../console/console.h"
#include "../world/world.h"

int WinMain(HINSTANCE app_handle, HINSTANCE prev_instance, LPSTR cmd_line, int show_cmd) {
    struct console console;

    if (!console__init_module(&console, 512)) {
        ExitProcess(EXIT_FAILURE);
    }

    struct world world;

    world__create(&world);

    world__destroy(&world);
    
    console__deinit_module(&console);
}
