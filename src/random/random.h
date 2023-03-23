#include "random_defs.h"

/*
    Copyright (C) 2004, Makoto Matsumoto and Takuji Nishimura,
    All rights reserved.

    source: http://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/emt.html
*/

struct random
{
    u64 state_vector[RANDOM__NN];
    u32 state_vector_index;
    u32 _reserved;
};
static_assert(sizeof(struct random) == 2504, "");

DLLEXPORT void random__init(struct random* self, u64 seed);

// @brief generates a random u64 on [0, 2^64-1]-interval
DLLEXPORT u64 random__u64(struct random* self);
// @brief generates a random u32 on [0, 2^32-1]-interval
DLLEXPORT u32 random__u32(struct random* self);
// @brief generates a random u32 on [0, 2^16-1]-interval
DLLEXPORT u16 random__u16(struct random* self);
// @brief generates a random i64 on [-2^63, 2^63-1]-interval
DLLEXPORT i64 random__i64(struct random* self);
// @brief generates a random i32 on [-2^31, 2^31-1]-interval
DLLEXPORT i32 random__i32(struct random* self);
// @brief generates a random i32 on [-2^15, 2^15-1]-interval
DLLEXPORT i16 random__i16(struct random* self);

// @brief generates a random r64 on [0,1]-real-interval
DLLEXPORT r64 random__r64(struct random* self);

// @brief generates a random i64 on a [left,right] interval
DLLEXPORT i64 random__i64_closed(struct random* self, i64 left, i64 right);
// @brief generates a random i32 on a [left,right] interval
DLLEXPORT i32 random__i32_closed(struct random* self, i32 left, i32 right);
// @brief generates a random i16 on a [left,right] interval
DLLEXPORT i16 random__i16_closed(struct random* self, i16 left, i16 right);
// @brief generates a random u64 on a [left,right] interval
DLLEXPORT u64 random__u64_closed(struct random* self, u64 left, u64 right);
// @brief generates a random u32 on a [left,right] interval
DLLEXPORT u32 random__u32_closed(struct random* self, u32 left, u32 right);
// @brief generates a random u16 on a [left,right] interval
DLLEXPORT u16 random__u16_closed(struct random* self, u16 left, u16 right);

// @brief generates a random r64 on [left,right]-real-interval
DLLEXPORT r64 random__r64_closed(struct random* self, r64 left, r64 right);
// @brief generates a random r32 on [left,right]-real-interval
DLLEXPORT r32 random__r32_closed(struct random* self, r32 left, r32 right);

// @brief generates a random r64 on [left,right)-real-interval
DLLEXPORT r64 random__r64_left_closed(struct random* self, r64 left, r64 right);
// @brief generates a random r32 on [left,right)-real-interval
DLLEXPORT r32 random__r32_left_closed(struct random* self, r32 left, r32 right);

// @brief generates a random r64 on (left,right)-real-interval
DLLEXPORT r64 random__r64_open(struct random* self, r64 left, r64 right);
// @brief generates a random r32 on (left,right)-real-interval
DLLEXPORT r32 random__r32_open(struct random* self, r32 left, r32 right);
