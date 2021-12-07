#pragma once

#include "framework64/camera.h"
#include "framework64/scene.h"
#include "framework64/engine.h"
#include "framework64/transform.h"

typedef enum {
    CAMERA_MODE_CHASE,
    CAMERA_MODE_SIDE,
    CAMERA_MODE_TOP,
    CAMERA_MODE_MANUAL
} CameraMode;

typedef struct {
    fw64Engine* engine;
    fw64Camera camera;
    fw64Transform* target;
    fw64Scene* scene;
    Vec3 last_target_position;
    int  target_transition;
    float current_collision_distance_adjustment;
    float target_bounding_height;
    float target_bounding_radius;
    float target_follow_dist;
    float target_follow_height;
    float target_forward_dist;
    float target_forward_height;
    float camera_adjust_speed;
    float collision_radius;

    Vec3 player_forward_dir;

    CameraMode mode, last_mode;
    int mode_changed, mode_transition; //flags indicating we need to lerp to new position
    Vec3 previous_mode_position, new_mode_ideal_position;
    float mode_transition_current_distance, mode_transition_total_distance;

} ChaseCamera;

#ifdef __cplusplus
extern "C" {
#endif

void chase_camera_init(ChaseCamera* chase_cam, fw64Engine* engine);
void chase_camera_reset(ChaseCamera* chase_cam, fw64Transform* target);
void chase_camera_update(ChaseCamera* chase_cam);
void chase_camera_set_mode(ChaseCamera* chase_cam, CameraMode new_mode, float new_follow_dist, float new_follow_height, float new_forward_dist, float new_forward_height);
void chase_camera_set_scene(ChaseCamera* chase_cam, fw64Scene* scene);
void chase_camera_get_forward(ChaseCamera* chase_cam, Vec3* out);

#ifdef __cplusplus
}
#endif