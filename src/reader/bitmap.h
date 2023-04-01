#pragma once

#include "reader_defs.h"
#include "../math/v2u32.h"

struct bitmap {
    u32          *data;
    struct v2u32 dims;
};
