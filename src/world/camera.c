#include "camera.h"

#include "sim_region.h"

void camera__create(
    struct camera* self,
    struct window* window,
    struct world* world,
    struct world_position viewport_center_p,
    struct v2r32 viewport_half_dims,
    struct v2r32 window_client_center_p,
    struct v2r32 window_client_half_dims
) {
    self->window = window;
    self->world = world;
    self->viewport_center_p = viewport_center_p;
    self->viewport_half_dims = viewport_half_dims;
    self->window_client_center_p = window_client_center_p;
    self->window_client_half_dims = window_client_half_dims;

    renderer__create(&self->renderer);
}

void camera__destroy(struct camera* self) {
    renderer__destroy(&self->renderer);
}

void camera__update_viewport_p_absolute(struct camera* self, struct world_position new_viewport_center_p) {
    self->viewport_center_p = new_viewport_center_p;
}

void camera__update_viewport_p_relative(struct camera* self, struct v2r32 viewport_center_dp) {
    self->viewport_center_p = world_position__from_relative_p(viewport_center_dp, self->viewport_center_p);
}

void camera__update_viewport_half_dims_absolute(struct camera* self, struct v2r32 new_viewport_half_dims) {
    self->viewport_half_dims = new_viewport_half_dims;
}

void camera__update_viewport_half_dims_relative(struct camera* self, struct v2r32 d_viewport_half_dims) {
    self->viewport_half_dims = v2r32__add_v2r32(self->viewport_half_dims, d_viewport_half_dims);
}

static inline void camera_render_entity_processor_callback(
    void* context,
    struct entity** entities,
    u32 entities_size
) {
    struct renderer* renderer = (struct renderer*) context;
    for (u32 entity_index = 0; entity_index < entities_size; ++entity_index) {
        struct entity* entity = entities[entity_index];
        if (entity != NULL) {
            if (entity->processor_callback_transient_values.is_in_sim_region) {
                renderer__push_rectangle_centered(
                    renderer,
                    entity->processor_callback_transient_values.relative_p,
                    entity->bounding_box_half_dims,
                    entity->color
                );
            }
        }
    }
}

void camera__render(struct camera* self) {
    sim_region__routine(
        self->world,
        &camera_render_entity_processor_callback,
        &self->renderer,
        self->viewport_center_p,
        self->viewport_half_dims
    );

    renderer__render(
        &self->renderer,
        self->window,
        self->world,
        self->viewport_center_p,
        self->viewport_half_dims,
        self->window_client_center_p,
        self->window_client_half_dims
    );

    renderer__clear_push_buffer(&self->renderer);
}
