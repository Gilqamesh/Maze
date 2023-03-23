#pragma once

#include "math_defs.h"

#include "v3u64.h"
#include "v3u32.h"
#include "v3u16.h"

#include "v2u64.h"
#include "v2u32.h"
#include "v2u16.h"

#include "v3i64.h"
#include "v3i32.h"
#include "v3i16.h"

#include "v2i64.h"
#include "v2i32.h"
#include "v2i16.h"

#include "v3r64.h"
#include "v3r32.h"

#include "v2r64.h"
#include "v2r32.h"

static inline u64 clamp__u64(u64 min, u64 value, u64 max) {
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

static inline u16 clamp__u16(u16 min, u16 value, u16 max) {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}

static inline i64 clamp__i64(i64 min, i64 value, i64 max) {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}

static inline i32 clamp__i32(i32 min, i32 value, i32 max) {
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}

static inline i16 clamp__i16(i16 min, i16 value, i16 max) {
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

static inline struct v3u64 clamp__v3u64(struct v3u64 min, struct v3u64 value, struct v3u64 max) {
    return v3u64(clamp__u64(min.x, value.x, max.x), clamp__u64(min.y, value.y, max.y), clamp__u64(min.z, value.z, max.z));
}

static inline struct v3u32 clamp__v3u32(struct v3u32 min, struct v3u32 value, struct v3u32 max) {
    return v3u32(clamp__u32(min.x, value.x, max.x), clamp__u32(min.y, value.y, max.y), clamp__u32(min.z, value.z, max.z));
}

static inline struct v3u16 clamp__v3u16(struct v3u16 min, struct v3u16 value, struct v3u16 max) {
    return v3u16(clamp__u16(min.x, value.x, max.x), clamp__u16(min.y, value.y, max.y), clamp__u16(min.z, value.z, max.z));
}

static inline struct v2u64 clamp__v2u64(struct v2u64 min, struct v2u64 value, struct v2u64 max) {
    return v2u64(clamp__u64(min.x, value.x, max.x), clamp__u64(min.y, value.y, max.y));
}

static inline struct v2u32 clamp__v2u32(struct v2u32 min, struct v2u32 value, struct v2u32 max) {
    return v2u32(clamp__u32(min.x, value.x, max.x), clamp__u32(min.y, value.y, max.y));
}

static inline struct v2u16 clamp__v2u16(struct v2u16 min, struct v2u16 value, struct v2u16 max) {
    return v2u16(clamp__u16(min.x, value.x, max.x), clamp__u16(min.y, value.y, max.y));
}

static inline struct v3i64 clamp__v3i64(struct v3i64 min, struct v3i64 value, struct v3i64 max) {
    return v3i64(clamp__i64(min.x, value.x, max.x), clamp__i64(min.y, value.y, max.y), clamp__i64(min.y, value.y, max.y));
}

static inline struct v3i32 clamp__v3i32(struct v3i32 min, struct v3i32 value, struct v3i32 max) {
    return v3i32(clamp__i32(min.x, value.x, max.x), clamp__i32(min.y, value.y, max.y), clamp__i32(min.z, value.z, max.z));
}

static inline struct v3i16 clamp__v3i16(struct v3i16 min, struct v3i16 value, struct v3i16 max) {
    return v3i16(clamp__i16(min.x, value.x, max.x), clamp__i16(min.y, value.y, max.y), clamp__i16(min.z, value.z, max.z));
}

static inline struct v2i64 clamp__v2i64(struct v2i64 min, struct v2i64 value, struct v2i64 max) {
    return v2i64(clamp__i64(min.x, value.x, max.x), clamp__i64(min.y, value.y, max.y));
}

static inline struct v2i32 clamp__v2i32(struct v2i32 min, struct v2i32 value, struct v2i32 max) {
    return v2i32(clamp__i32(min.x, value.x, max.x), clamp__i32(min.y, value.y, max.y));
}

static inline struct v2i16 clamp__v2i16(struct v2i16 min, struct v2i16 value, struct v2i16 max) {
    return v2i16(clamp__i16(min.x, value.x, max.x), clamp__i16(min.y, value.y, max.y));
}

static inline struct v3r64 clamp__v3r64(struct v3r64 min, struct v3r64 value, struct v3r64 max) {
    return v3r64(clamp__r64(min.x, value.x, max.x), clamp__r64(min.y, value.y, max.y), clamp__r64(min.z, value.z, max.z));
}

static inline struct v3r32 clamp__v3r32(struct v3r32 min, struct v3r32 value, struct v3r32 max) {
    return v3r32(clamp__r32(min.x, value.x, max.x), clamp__r32(min.y, value.y, max.y), clamp__r32(min.z, value.z, max.z));
}

static inline struct v2r64 clamp__v2r64(struct v2r64 min, struct v2r64 value, struct v2r64 max) {
    return v2r64(clamp__r64(min.x, value.x, max.x), clamp__r64(min.y, value.y, max.y));
}

static inline struct v2r32 clamp__v2r32(struct v2r32 min, struct v2r32 value, struct v2r32 max) {
    return v2r32(clamp__r32(min.x, value.x, max.x), clamp__r32(min.y, value.y, max.y));
}
