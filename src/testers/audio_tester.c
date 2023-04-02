#include "../audio/audio.h"
#include "../window/window.h"

int WinMain(HINSTANCE app_handle, HINSTANCE prev_instance, LPSTR cmd_line, int show_cmd) {
    struct console console;
    console__init_module(&console, 256);

    struct window window;
    window__init_module(&window, &console);
    window__create(&window, app_handle, "Audio Tester Window", v2u32(100, 100), v2u32(800, 600));

    struct audio_module audio_module;
    if (audio__init_module(&audio_module) == false) {
        ExitProcess(1);
    }

    const char* wav_file_path = "../assets/634.wav";
    const char* wav_file_path_punch_1 = "../assets/punch-1.wav";
    struct audio audio_ambient;
    struct audio audio_punch_1;
    if (audio__create(&audio_ambient, &audio_module, wav_file_path, true) == false ||
        audio__create(&audio_punch_1, &audio_module, wav_file_path_punch_1, false) == false
    ) {
        ExitProcess(2);
    }

    if (audio__start(&audio_ambient) == false
    ) {
        ExitProcess(3);
    }

    audio__set_volume(&audio_ambient, -15.0f);

    while (window__does_exist(&window)) {
        window__poll_inputs(&window);
        window__clear_screen(&window, COLOR_WHITE);

        r32 decibels = audio__get_volume(&audio_ambient);
        r32 ddecibels = 0.1f;
        r32 freq_ratio = audio__get_frequency_ratio(&audio_ambient);
        r32 dfreq_ratio = 0.001f;
        if (window__key_is_down(&window, KEY_UP)) {
            audio__set_volume(&audio_ambient, decibels + ddecibels);
        }
        if (window__key_is_down(&window, KEY_DOWN)) {
            audio__set_volume(&audio_ambient, decibels - ddecibels);
        }
        if (window__key_is_down(&window, KEY_RIGHT)) {
            audio__set_frequency_ratio(&audio_ambient, freq_ratio + dfreq_ratio);
        }
        if (window__key_is_down(&window, KEY_LEFT)) {
            audio__set_frequency_ratio(&audio_ambient, freq_ratio - dfreq_ratio);
        }
        if (window__key_is_pressed(&window, KEY_SPACE)) {
            audio__start(&audio_punch_1);
        }
        console__log(&console, "decibels: %f, freq_ratio: %f\n", decibels, freq_ratio);
        window__end_draw(&window);
    }

    audio__destroy(&audio_ambient);
    audio__destroy(&audio_punch_1);

    audio__deinit_module(&audio_module);

    console__deinit_module(&console);
}
