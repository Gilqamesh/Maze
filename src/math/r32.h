#pragma once

#include "math_defs.h"

r32 r32__fractional_part(r32 r);

static inline r32 r32__integer_part(r32 r) {
    return (i32)r;
}

static inline r32 r32__fractional_part(r32 r) {
    return r - r32__integer_part(r);
}
