#pragma once

#include "window_defs.h"

#define RED_OFFSET    16
#define GREEN_OFFSET  8
#define BLUE_OFFSET   0
#define ALPHA_OFFSET  24

// ARGB
enum colors {
    COLOR_RED    = 0xffff0000,
    COLOR_GREEN  = 0xff00ff00,
    COLOR_BLUE   = 0xff0000ff,
    COLOR_YELLOW = 0xffffff00,
    COLOR_PURPLE = 0xffff00ff,
    COLOR_CYAN   = 0xff00ffff,
    COLOR_WHITE  = 0xffffffff,
    COLOR_GRAY   = 0xffc8c8c8,
    COLOR_BLACK  = 0xff000000
};

static inline u32 color(u32 red, u32 green, u32 blue, u32 alpha) {
    return
        (red & 255)   << RED_OFFSET   ||
        (green & 255) << GREEN_OFFSET ||
        (blue & 255)  << BLUE_OFFSET  ||
        (alpha & 255) << ALPHA_OFFSET;
}
