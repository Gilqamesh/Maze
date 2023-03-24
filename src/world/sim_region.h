#pragma once

#include "world_defs.h"

#include "world.h"
#include "entity.h"
#include "../math/v2u32.h"

/*
    -------------------------------------------------------------------
    |     |     |     |     |     |     |     |     |     |     |     |
    |     |     |     |     |     |     |     |     |     |     |     |
    -------------------------------------------------------------------
    |     |     |     |     |     |     |     |     |     |     |     |
    |     |     |   |/////////////////////////////| |     |     |     |
    ----------------|/////////////////////////////|--------------------
    |     |     |   |/////////////////////////////| |     |     |     |
    |     |     |   |/////////////////////////////| |     |     |     |
    ----------------|//////    Sim Region   //////|--------------------
    |     |     |   |/////////////////////////////| |     |     |     |
    |     |     |   |/// pulls in all entities ///| |     |     |     |
    ----------------|// from nearby world grids //|--------------------
    |     |     |   |////////////////////////// //| |     |     |     |
    |     |     |   |/////////////////////////////| |     |     |     |
    -------------------------------------------------------------------
    |     |     |     |     |     |     |     |     |     |     |     |
    |     |     |     |     |     |     |     |     |     |     |     |
    -------------------------------------------------------------------
    |     |     |     |     |     |     |     |     |     |     |     |
    |     |     |     |     |     |     |     |     |     |     |     |
    -------------------------------------------------------------------
*/

struct sim_region {
    struct world_position center_p;
    struct v2u32 dims;
};

typedef void (*sim_region__entity_processor_callback)(struct entity** entities, u32 entities_size, struct world_position relative_p);

// @brief pulls in all the entities from surrounding world_grids into the sim_region
DLLEXPORT void sim_region__routine(
    struct sim_region* self,
    struct world* world,
    sim_region__entity_processor_callback entity_processor_callback,
    struct world_position p,
    struct v2u32 dims
);
