#pragma once

#include <Windows.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

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
#define ArraySize(Array) (sizeof(Array)/sizeof((Array)[0]))

enum APP_ERROR {
    APP_ERROR_WINDOW_CALLBACK
};
