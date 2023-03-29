#pragma once

#include "window_defs.h"

#define RED_OFFSET    16
#define GREEN_OFFSET  8
#define BLUE_OFFSET   0
#define ALPHA_OFFSET  24

// ARGB
enum colors {
    COLOR_RED    = 0x00ff0000,
    COLOR_GREEN  = 0x0000ff00,
    COLOR_BLUE   = 0x000000ff,
    COLOR_YELLOW = 0x00ffff00,
    COLOR_PURPLE = 0x00ff00ff,
    COLOR_CYAN   = 0x0000ffff,
    COLOR_WHITE  = 0x00ffffff,
    COLOR_GRAY   = 0x00c8c8c8,
    COLOR_BLACK  = 0x00000000
};

static inline u32 color(u32 red, u32 green, u32 blue, u32 alpha) {
    return
        (red & 255)   << RED_OFFSET   ||
        (green & 255) << GREEN_OFFSET ||
        (blue & 255)  << BLUE_OFFSET  ||
        (alpha & 255) << ALPHA_OFFSET;
}
