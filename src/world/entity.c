#include "entity.h"

struct entity* entity__create(struct world_position p, struct v2u32 bounding_box_dims) {
    struct entity* result = (struct entity*) calloc(1, sizeof(*result));

    result->p = p;
    result->bounding_box_dims = bounding_box_dims;

    return result;
}

void entity__destroy(struct entity* self) {
    (void) self;
}
