#pragma once

#include "math_defs.h"

static inline u32 lerp__u32(u32 a, r32 p, u32 b) {
    return (u32) ((r32) a + p * ((r32) b - (r32) a));
}

static inline u32 lerp__rgba(u32 a, r32 p, u32 b) {
    // unpack
    u32 blue_channel_a  = (a >> BLUE_OFFSET)  & 0xff;
    u32 green_channel_a = (a >> GREEN_OFFSET) & 0xff;
    u32 red_channel_a   = (a >> RED_OFFSET)   & 0xff;
    u32 alpha_channel_a = (a >> ALPHA_OFFSET) & 0xff;

    u32 blue_channel_b  = (b >> BLUE_OFFSET)  & 0xff;
    u32 green_channel_b = (b >> GREEN_OFFSET) & 0xff;
    u32 red_channel_b   = (b >> RED_OFFSET)   & 0xff;
    u32 alpha_channel_b = (b >> ALPHA_OFFSET) & 0xff;

    // lerp
    u32 result_blue_channel  = lerp__u32(blue_channel_a,  p, blue_channel_b);
    u32 result_green_channel = lerp__u32(green_channel_a, p, green_channel_b);
    u32 result_red_channel   = lerp__u32(red_channel_a,   p, red_channel_b);
    u32 result_alpha_channel = lerp__u32(alpha_channel_a, p, alpha_channel_b);

    // pack
    u32 result = (
        result_blue_channel  << BLUE_OFFSET  ||
        result_green_channel << GREEN_OFFSET ||
        result_red_channel   << RED_OFFSET   ||
        result_alpha_channel << ALPHA_OFFSET
    );

    return result;
}
