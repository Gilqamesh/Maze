#include "camera.h"

#include "sim_region.h"

void camera__create(
    struct camera* self,
    struct window* window,
    struct world* world,
    struct world_position viewport_center_p,
    struct v3r32 viewport_half_dims,
    struct v2r32 window_client_top_left_p_normalized,
    struct v2r32 window_client_bot_right_p_normalized
) {
    self->window = window;
    self->world = world;
    self->viewport_center_p = viewport_center_p;
    self->viewport_half_dims = viewport_half_dims;
    self->window_client_top_left_p_normalized = window_client_top_left_p_normalized;
    self->window_client_bot_right_p_normalized = window_client_bot_right_p_normalized;

    renderer__create(&self->renderer);
}

void camera__destroy(struct camera* self) {
    renderer__destroy(&self->renderer);
}

void camera__update_viewport_p_absolute(struct camera* self, struct world_position new_viewport_center_p) {
    self->viewport_center_p = new_viewport_center_p;
}

void camera__update_viewport_p_relative(struct camera* self, struct v3r32 viewport_center_dp) {
    self->viewport_center_p = world_position__from_relative_p(viewport_center_dp, self->viewport_center_p);
}

void camera__update_viewport_half_dims_absolute(struct camera* self, struct v3r32 new_viewport_half_dims) {
    self->viewport_half_dims = new_viewport_half_dims;
}

void camera__update_viewport_half_dims_relative(struct camera* self, struct v3r32 d_viewport_half_dims) {
    self->viewport_half_dims = v3r32__add_v3r32(self->viewport_half_dims, d_viewport_half_dims);
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
            if (entity__flag_is_set(entity, ENTITY_FLAGS_IS_IN_SIM_REGION)) {
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

bool camera__is_p_in_window_client_area(struct camera* self, struct v2r32 p) {
    p = v2r32(
        p.x / (r32) self->window->dims.x,
        p.y / (r32) self->window->dims.y
    );
    if (p.x < self->window_client_top_left_p_normalized.x ||
        p.x > self->window_client_bot_right_p_normalized.x ||
        p.y < self->window_client_top_left_p_normalized.y || 
        p.y > self->window_client_bot_right_p_normalized.y
    ) {
        return false;
    }

    return true;
}

void camera__render(struct camera* self) {
    sim_region__routine(
        self->world,
        &camera_render_entity_processor_callback,
        &self->renderer,
        self->viewport_center_p,
        self->viewport_half_dims
    );

    push_buffer__sort_rectangles(&self->renderer.push_buffer);

    renderer__render(
        &self->renderer,
        self->window,
        self->world,
        self->viewport_center_p,
        self->viewport_half_dims,
        self->window_client_top_left_p_normalized,
        self->window_client_bot_right_p_normalized
    );

    renderer__clear_push_buffer(&self->renderer);
}
