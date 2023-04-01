#include "../audio/audio.h"
#include "../window/window.h"

int WinMain(HINSTANCE app_handle, HINSTANCE prev_instance, LPSTR cmd_line, int show_cmd) {
    struct console console;
    console__init_module(&console, 256);

    struct window window;
    window__init_module(&window, &console);
    window__create(&window, app_handle, "Audio Tester Window", v2u32(100, 100), v2u32(800, 600));

    if (audio__init_module() == false) {
        ExitProcess(1);
    }

    const char* wav_file_path = "../assets/634.wav";
    struct audio audio;
    if (audio__create(&audio, wav_file_path, true) == false) {
        ExitProcess(2);
    }

    if (audio__start(&audio) == false) {
        ExitProcess(3);
    }

    while (window__does_exist(&window)) {
        window__poll_inputs(&window);
        window__clear_screen(&window, COLOR_WHITE);

        r32 decibels = audio__get_volume(&audio);
        r32 ddecibels = 0.1f;
        r32 freq_ratio = audio__get_frequency_ratio(&audio);
        r32 dfreq_ratio = 0.001f;
        if (window__key_is_down(&window, KEY_UP)) {
            audio__set_volume(&audio, decibels + ddecibels);
        }
        if (window__key_is_down(&window, KEY_DOWN)) {
            audio__set_volume(&audio, decibels - ddecibels);
        }
        if (window__key_is_down(&window, KEY_RIGHT)) {
            audio__set_frequency_ratio(&audio, freq_ratio + dfreq_ratio);
        }
        if (window__key_is_down(&window, KEY_LEFT)) {
            audio__set_frequency_ratio(&audio, freq_ratio - dfreq_ratio);
        }
        console__log(&console, "decibels: %f, freq_ratio: %f\n", decibels, freq_ratio);
        window__end_draw(&window);
    }

    audio__destroy(&audio);

    audio__deinit_module();

    console__deinit_module(&console);
}
