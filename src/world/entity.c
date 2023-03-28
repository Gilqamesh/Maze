#include "entity.h"

struct entity* entity__create_absolute(
    struct world_position center_p,
    struct v2r32 bounding_box_half_dims,
    enum color color
) {
    struct entity* result = (struct entity*) calloc(1, sizeof(*result));
    if (result == NULL) {
        ExitProcess(APP_ERROR_ALLOC_FAIL);
    }

    result->center_p = center_p;
    result->bounding_box_half_dims = bounding_box_half_dims;
    result->color = color;

    return result;
}

struct entity* entity__create_relative(
    struct v2r32 center_p,
    struct world_position relative_p,
    struct v2r32 bounding_box_half_dims,
    enum color color
) {
    struct entity* result = (struct entity*) calloc(1, sizeof(*result));
    if (result == NULL) {
        ExitProcess(APP_ERROR_ALLOC_FAIL);
    }

    result->center_p = world_position__from_relative_p(center_p, relative_p);
    result->bounding_box_half_dims = bounding_box_half_dims;
    result->color = color;

    return result;
}

void entity__destroy(struct entity* self) {
    (void) self;
}
