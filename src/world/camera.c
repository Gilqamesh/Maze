#include "camera.h"

#include "sim_region.h"

void camera__create(
    struct camera* self,
    struct window* window,
    struct world* world,
    struct world_position view_port_center_p,
    struct v2r32 view_port_half_dims,
    struct v2u32 window_client_top_left_p,
    struct v2u32 window_client_dims
) {
    self->window = window;
    self->world = world;
    self->view_port_center_p = view_port_center_p;
    self->view_port_half_dims = view_port_half_dims;
    self->window_client_top_left_p = window_client_top_left_p;
    self->window_client_dims = window_client_dims;

    renderer__create(&self->renderer);
}

void camera__destroy(struct camera* self) {
    renderer__destroy(&self->renderer);
}

void camera__update_view_port_p_absolute(struct camera* self, struct world_position new_view_port_center_p) {
    self->view_port_center_p = new_view_port_center_p;
}

void camera__update_view_port_p_relative(struct camera* self, struct v2r32 view_port_center_dp) {
    self->view_port_center_p = world_position__from_relative_p(view_port_center_dp, self->view_port_center_p);
}

void camera__update_view_port_half_dims_absolute(struct camera* self, struct v2r32 new_view_port_half_dims) {
    self->view_port_half_dims = new_view_port_half_dims;
}

void camera__update_view_port_half_dims_relative(struct camera* self, struct v2r32 d_view_port_half_dims) {
    self->view_port_half_dims = v2r32__add_v2r32(self->view_port_half_dims, d_view_port_half_dims);
}

static inline void camera_render_entity_processor_callback(
    struct entity** entities,
    void* context,
    u32 entities_size
) {
    struct renderer* renderer = (struct renderer*) context;
    for (u32 entity_index = 0; entity_index < entities_size; ++entity_index) {
        struct entity* entity = entities[entity_index];
        if (entity != NULL) {
            if (entity->processor_callback_transient_values.is_in_sim_region &&
                entity__flag_is_set(entity, ENTITY_FLAGS__IS_RENDEREABLE)) {
                renderer__push_rectangle(
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
        self->view_port_center_p,
        self->view_port_half_dims
    );

    renderer__render(
        &self->renderer,
        self->window,
        self->world,
        self->window_client_top_left_p,
        self->window_client_dims
    );

    renderer__clear_push_buffer(&self->renderer);
}
