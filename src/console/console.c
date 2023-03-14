#include "console.h"

bool console__init_module(struct console* self, u32 max_message_length)
{
    // TODO(david): diagnostics
    if (!AttachConsole(ATTACH_PARENT_PROCESS)) {
        return false;
    }

    if ((self->out_handle = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE) {
        return false;
    }

    if ((self->err_handle = GetStdHandle(STD_ERROR_HANDLE )) == INVALID_HANDLE_VALUE) {
        return false;
    }

    self->buffer_size = max_message_length;
    self->buffer = (char*) malloc(self->buffer_size);
    if (self->buffer == NULL) {
        return false;
    }

    return true;
}

void console__deinit_module(struct console* self)
{
    FreeConsole();

    self->out_handle = INVALID_HANDLE_VALUE;
    self->err_handle = INVALID_HANDLE_VALUE;

    if (self->buffer) {
        free(self->buffer);
    }
}

void console__log(struct console* self, char* msg, ...)
{
    if (self->out_handle != INVALID_HANDLE_VALUE) {
        va_list ap;
        va_start(ap, msg);

        if (self->buffer_size <= vsnprintf(self->buffer, self->buffer_size, msg, ap)) {
            // todo(david): diagnostic, truncated msg
        }

        DWORD bytes_written;
        if (WriteConsoleA(self->out_handle, self->buffer, strnlen(self->buffer, self->buffer_size), &bytes_written, NULL) == 0) {
            // TODO(david): diagnostic
        }

        va_end(ap);
    }
}

void console__fatal(struct console* self, char *error_msg, ...)
{
    if (self->err_handle != INVALID_HANDLE_VALUE) {
        va_list ap;
        va_start(ap, error_msg);

        if (self->buffer_size <= vsnprintf(self->buffer, self->buffer_size, error_msg, ap)) {
            // todo(david): diagnostic, truncated msg
        }

        DWORD bytes_written;
        WriteConsoleA(self->err_handle, self->buffer, strnlen(self->buffer, self->buffer_size), &bytes_written, NULL);

        va_end(ap);
    }

    console__deinit_module(self);

    ExitProcess(EXIT_FAILURE);
}
