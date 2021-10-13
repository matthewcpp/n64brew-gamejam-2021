#include "chase_camera.h"

#define DEFAULT_CAMERA_BACK_FOLLOW_DIST 9.0f
#define DEFAULT_CAMERA_FORWARD_TARGET_DIST 5.0f
#define DEFAULT_CAMERA_FOLLOW_HEIGHT 10.0f

void chase_camera_init(ChaseCamera* chase_cam, fw64Node* target) {
    fw64_camera_init(&chase_cam->camera);
    chase_cam->camera.near = 1.0f;
    chase_cam->camera.far = 100.0f;
    fw64_camera_update_projection_matrix(&chase_cam->camera);

    chase_cam->target = target;

    chase_cam->back_follow_dist = DEFAULT_CAMERA_BACK_FOLLOW_DIST;
    chase_cam->forward_target_dist = DEFAULT_CAMERA_FORWARD_TARGET_DIST;
    chase_cam->follow_height = DEFAULT_CAMERA_FOLLOW_HEIGHT;
}

void chase_camera_update(ChaseCamera* chase_cam) {
    Vec3 back, forward, camera_pos, camera_target;
    Vec3 up = {0.0, 1.0, 0.0};
    fw64_transform_back(&chase_cam->target->transform, &back);
    vec3_copy(&forward, &back);
    vec3_negate(&forward);

    vec3_scale(&back, &back, chase_cam->back_follow_dist );
    vec3_add(&camera_pos, &chase_cam->target->transform.position, &back);
    camera_pos.y += chase_cam->follow_height;

    vec3_scale(&forward, &forward, chase_cam->forward_target_dist);
    vec3_add(&camera_target, &chase_cam->target->transform.position, &forward);

    chase_cam->camera.transform.position = camera_pos;
    fw64_transform_look_at(&chase_cam->camera.transform, &camera_target, &up);
    fw64_camera_update_view_matrix(&chase_cam->camera);
}