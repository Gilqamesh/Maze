#pragma once

#include <Windows.h>

// TOOD(david): replace libc with own implementation
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef int64_t  i64;
typedef int32_t  i32;
typedef int16_t  i16;
typedef int8_t   i8;

typedef float  r32;
typedef double r64;

#define DLLEXPORT __declspec(dllexport)
#define ARRAY_SIZE(array) (sizeof(array)/sizeof((array)[0]))

enum ERROR_START_CODE {
    ERROR_START_CODE_APP = 1,
    ERROR_START_CODE_FILE_READER = 100
};

enum APP_ERROR {
    APP_ERROR_WINDOW_CALLBACK = ERROR_START_CODE_APP,
    APP_ERROR_CONSOLE_UNINITIALIZED,
    APP_ERROR_WM_CREATE,
    APP_ERROR_INVALID_CODE_PATH,
    APP_ERROR_ALLOC_FAIL,
    APP_ERROR_RAN_OUT_OF_MEMORY,
    APP_CONSOLE_FATAL
};
