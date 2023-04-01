#pragma once

#include "console_defs.h"

struct console { // ascii console for window applications
    HANDLE out_handle;
    HANDLE err_handle;

    char   *buffer;
    u32    buffer_size;
};

DLLEXPORT bool console__init_module(struct console* self, u32 max_message_length);
DLLEXPORT void console__deinit_module(struct console* self);

DLLEXPORT void console__log(struct console* self, char* msg, ...);

// @brief use to exit the application process in case something goes wrong..
DLLEXPORT void console__fatal(struct console* self, char *error_msg, ...);

// todo: implement error levels, remove fatal
DLLEXPORT void console__error(struct console* self, char *error_msg, ...);
