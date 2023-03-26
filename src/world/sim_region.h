#pragma once

#include "world_defs.h"

#include "world.h"
#include "entity.h"
#include "renderer.h"
#include "../math/v2r32.h"

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

typedef void (*sim_region__entity_processor_callback)(
    struct entity** entities,
    u32 entities_size
);

// @brief pulls in all the entities from surrounding world_grids into the sim_region
DLLEXPORT void sim_region__routine(
    struct world* world,
    sim_region__entity_processor_callback entity_processor_callback,
    struct world_position center_p,
    struct v2r32 half_dims
);
