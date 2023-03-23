#pragma once

#include "../defs.h"

static inline r32 world__meters_to_pixels(r32 meters) {
    return meters * 16.0f;
}

static inline r32 world__pixels_to_meters(r32 pixels) {
    return pixels / 16.0f;
}
