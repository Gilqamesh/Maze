#pragma once

#include "math_defs.h"
#include "v2u32.h"
#include "v2r32.h"

static inline u16 clamp__u16(u16 min, u16 value, u16 max) {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}

static inline u32 clamp__u32(u32 min, u32 value, u32 max) {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}

static inline r32 clamp__r32(r32 min, r32 value, r32 max) {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}

static inline struct v2u16 clamp__v2u16(struct v2u16 min, struct v2u16 value, struct v2u16 max) {
    return v2u16(clamp__u16(min.x, value.x, max.x), clamp__u16(min.y, value.y, max.y));
}

static inline struct v2u32 clamp__v2u32(struct v2u32 min, struct v2u32 value, struct v2u32 max) {
    return v2u32(clamp__u32(min.x, value.x, max.x), clamp__u32(min.y, value.y, max.y));
}

static inline struct v2r32 clamp__v2r32(struct v2r32 min, struct v2r32 value, struct v2r32 max) {
    return v2r32(clamp__r32(min.x, value.x, max.x), clamp__r32(min.y, value.y, max.y));
}
