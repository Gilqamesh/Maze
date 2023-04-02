#include "../reader/file_reader.h"
#include "../console/console.h"

int WinMain(HINSTANCE app_handle, HINSTANCE prev_instance, LPSTR cmd_line, int show_cmd) {
    struct console  console;
    if (console__init_module(&console, 256) == false) {
        // ExitProcess(APP_ERROR_CONSOLE_UNINITIALIZED);
    }

    struct file_reader  file_reader;
    const char* file_path = "assets/text.txt";
    if (file_reader__create(&file_reader, file_path) == false) {
        console__fatal(&console, "failed to open the file: %s\n", file_path);
    }

    char buffer[128];
    u32 read_bytes = 0;
    while (file_reader__eof_reached(&file_reader) == false) {
        read_bytes = file_reader__read(&file_reader, buffer, sizeof(buffer) - 1);
        buffer[read_bytes] = '\0';
    }


    console__log(&console, "%s\n", buffer);
    console__log(&console, "Read bytes: %u\n", read_bytes);

    console__deinit_module(&console);
}
