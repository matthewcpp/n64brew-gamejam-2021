#include "chase_camera.h"
#include "layermap.h"
#include <framework64/n64/controller_button.h>

#include <stddef.h>

#define CAMERA_FOLLOW_DIST 30.0f
#define CAMERA_FOLLOW_HEIGHT 12.0f
#define CAMERA_FORWARD_TARGET_DIST 0.0f
#define CAMERA_FORWARD_TARGET_HEIGHT 0.0f
#define CAMERA_ADJUST_SPEED 20.0f
#define CAMERA_COLLISION_RADIUS 1.0f
#define CAMERA_PROJ_MULT_H 0.27614f //magic math
#define CAMERA_PROJ_MULT_V 0.20711f //same

void chase_camera_init(ChaseCamera* chase_cam, fw64Engine* engine) {
    fw64_camera_init(&chase_cam->camera);
    chase_cam->camera.near = 1.5f;
    chase_cam->camera.far = 250.0f;
    fw64_camera_update_projection_matrix(&chase_cam->camera);

    chase_cam->engine = engine;
    chase_cam->target = NULL;
    chase_cam->scene  = NULL;
    vec3_zero(&chase_cam->last_target_position);
    chase_cam->target_bounding_height = CAMERA_FOLLOW_DIST * CAMERA_PROJ_MULT_V;
    chase_cam->target_bounding_radius = CAMERA_FOLLOW_DIST * CAMERA_PROJ_MULT_H;
    chase_cam->target_follow_dist = CAMERA_FOLLOW_DIST;
    chase_cam->target_follow_height = CAMERA_FOLLOW_HEIGHT;
    chase_cam->target_forward_dist = CAMERA_FORWARD_TARGET_DIST;
    chase_cam->target_forward_height = CAMERA_FORWARD_TARGET_HEIGHT;
    chase_cam->camera_adjust_speed = CAMERA_ADJUST_SPEED;
    chase_cam->collision_radius = CAMERA_COLLISION_RADIUS;
    chase_cam->mode = CAMERA_MODE_CHASE;
}

static void chase_camera_update_position(ChaseCamera* chase_cam) {
    //only update in the right camera mode with good data
    if (!chase_cam->target) return;
    if (!chase_cam->scene)  return;
    if (chase_cam->mode == CAMERA_MODE_MANUAL) return;

    //setup basic variables
    Vec3 vec_up = {0.0f, 1.0f, 0.0f};
    Vec3 camera_pos, camera_target;
    Vec3 vec_camera_settings_offset;
    vec3_zero(&vec_camera_settings_offset); //chase_cam->target_follow_height, chase_cam->target_follow_dist       
    vec3_copy(&camera_pos, &chase_cam->camera.transform.position);
    vec3_copy(&camera_target, &chase_cam->last_target_position);

    float* camera_target_left_right;
    float* camera_target_forward_back;
    float* camera_target_up_down;

    float* player_left_right;
    float* player_forward_back;
    float* player_up_down;

    float  left_right_bounding;
    float  forward_back_bounding;
    float  up_down_bounding;

    float* camera_offset_back;
    float* camera_offset_up;

    //this is really dumb, but it sets the axes correctly for the 3 different camera orientations
    //then we use the same conditions to translate the camera correctly for all orientations
    //todo: use actual math to just make everything relative to the camera plane
    switch(chase_cam->mode) {
        case(CAMERA_MODE_TOP):
            camera_target_left_right   = &camera_target.x;
            player_left_right          = &chase_cam->target->position.x;
            left_right_bounding        =  chase_cam->target_bounding_radius;

            camera_target_forward_back = &camera_target.y;
            player_forward_back        = &chase_cam->target->position.y;
            forward_back_bounding      =  chase_cam->target_bounding_height;

            camera_target_up_down      = &camera_target.z;
            player_up_down             = &chase_cam->target->position.z;
            up_down_bounding           =  chase_cam->target_bounding_height;

            camera_offset_back         = &vec_camera_settings_offset.y;
            camera_offset_up           = &vec_camera_settings_offset.z;
            break;
        case(CAMERA_MODE_SIDE):
            camera_target_left_right   = &camera_target.z;
            player_left_right          = &chase_cam->target->position.z;
            left_right_bounding        =  chase_cam->target_bounding_height;

            camera_target_forward_back = &camera_target.x;
            player_forward_back        = &chase_cam->target->position.x;
            forward_back_bounding      =  chase_cam->target_bounding_radius;

            camera_target_up_down      = &camera_target.y;
            player_up_down             = &chase_cam->target->position.y;
            up_down_bounding           =  chase_cam->target_bounding_height;

            camera_offset_back         = &vec_camera_settings_offset.x;
            camera_offset_up           = &vec_camera_settings_offset.y;
            break;
        case(CAMERA_MODE_CHASE): /* fall through */
        default:
            camera_target_left_right   = &camera_target.x;
            player_left_right          = &chase_cam->target->position.x;
            left_right_bounding        =  chase_cam->target_bounding_radius;

            camera_target_forward_back = &camera_target.z;
            player_forward_back        = &chase_cam->target->position.z;
            forward_back_bounding      =  chase_cam->target_bounding_height;

            camera_target_up_down      = &camera_target.y;
            player_up_down             = &chase_cam->target->position.y;
            up_down_bounding           =  chase_cam->target_bounding_height;

            camera_offset_back         = &vec_camera_settings_offset.z;
            camera_offset_up           = &vec_camera_settings_offset.y;
            break;
    }

    //translate left or right
    if(*camera_target_left_right < *player_left_right) {
        if(*player_left_right - *camera_target_left_right > left_right_bounding) {
            *camera_target_left_right += (*player_left_right - *camera_target_left_right - left_right_bounding);
        }
    }
    else {
        if(*camera_target_left_right - *player_left_right > left_right_bounding) {
            *camera_target_left_right -= (*camera_target_left_right - *player_left_right - left_right_bounding);
        }
    }

    //translate forward or backward
    if(*camera_target_forward_back < *player_forward_back) {
        if(*player_forward_back - *camera_target_forward_back > forward_back_bounding) {
            *camera_target_forward_back += (*player_forward_back - *camera_target_forward_back - forward_back_bounding);
        }
    }
    else {
        if(*camera_target_forward_back - *player_forward_back > forward_back_bounding) {
            *camera_target_forward_back -= (*camera_target_forward_back - *player_forward_back - forward_back_bounding);
        }
    }

    //translate up or down
    float y_val = (up_down_bounding / 2) + *player_up_down;
    if(*camera_target_up_down < y_val) {
        if(y_val - *camera_target_up_down > up_down_bounding) {
            *camera_target_up_down += (y_val - *camera_target_up_down - up_down_bounding);
        }
    }
    else {
        if(*camera_target_up_down - y_val > up_down_bounding) {
            *camera_target_up_down -= (*camera_target_up_down - y_val - up_down_bounding);
        }
    }

    //offset camera from target
    *camera_offset_up   = chase_cam->target_follow_height;
    *camera_offset_back = chase_cam->target_follow_dist;
    vec3_add(&camera_pos, &camera_target, &vec_camera_settings_offset);

    //detect scene geomatry blocking line of sight and move forward if needed
    fw64RaycastHit ray_hit;
    Vec3 ray_dir;
    float min_camera_dist = 1.0f;
    vec3_subtract(&ray_dir, &camera_pos, &camera_target);
    vec3_normalize(&ray_dir);
    fw64_scene_raycast(chase_cam->scene, &camera_target, &ray_dir, ~(NODE_LAYER_GROUND), &ray_hit);
    float camera_total_dist = vec3_distance(&camera_target, &camera_pos);
    if(ray_hit.distance < camera_total_dist) {
        vec3_subtract(&ray_dir, &ray_hit.point, &camera_pos);
        vec3_normalize(&ray_dir);
        vec3_scale(&ray_dir, &ray_dir, (camera_total_dist - ray_hit.distance) + chase_cam->collision_radius);
        vec3_add(&camera_pos, &camera_pos, &ray_dir);
    }

    //persist work to actual camera state
    vec3_copy(&chase_cam->camera.transform.position, &camera_pos);
    vec3_copy(&chase_cam->last_target_position, &camera_target);

    //update screen space player boundaries
    camera_total_dist = vec3_distance(&chase_cam->last_target_position, &chase_cam->camera.transform.position);
    chase_cam->target_bounding_height = camera_total_dist * CAMERA_PROJ_MULT_V;
    chase_cam->target_bounding_radius = camera_total_dist * CAMERA_PROJ_MULT_H;

    //update camera matrices
    fw64_transform_look_at(&chase_cam->camera.transform, &camera_target, &vec_up);
    fw64_camera_update_view_matrix(&chase_cam->camera);
}

static void chase_camera_update_input(ChaseCamera* chase_cam) {
    if (fw64_input_button_down(chase_cam->engine->input, 0, FW64_N64_CONTROLLER_BUTTON_R)) {
        chase_cam->target_follow_height += chase_cam->camera_adjust_speed * chase_cam->engine->time->time_delta;
    }

    if (fw64_input_button_down(chase_cam->engine->input, 0, FW64_N64_CONTROLLER_BUTTON_L)) {
        chase_cam->target_follow_height -= chase_cam->camera_adjust_speed * chase_cam->engine->time->time_delta;
    }

    if (fw64_input_button_down(chase_cam->engine->input, 0, FW64_N64_CONTROLLER_BUTTON_C_UP)) {
        chase_cam->target_follow_dist += chase_cam->camera_adjust_speed * chase_cam->engine->time->time_delta;
    }

    if (fw64_input_button_down(chase_cam->engine->input, 0, FW64_N64_CONTROLLER_BUTTON_C_DOWN)) {
        chase_cam->target_follow_dist -= chase_cam->camera_adjust_speed * chase_cam->engine->time->time_delta;
    }
}

void chase_camera_update(ChaseCamera* chase_cam) {
    chase_camera_update_position(chase_cam);
    chase_camera_update_input(chase_cam);
}

void chase_camera_set_scene(ChaseCamera* chase_cam, fw64Scene* scene) {
    chase_cam->scene = scene;
}

void chase_camera_reset(ChaseCamera* chase_cam, fw64Transform* target) {
    chase_cam->target = target;
    vec3_copy(&chase_cam->last_target_position, &target->position);
    vec3_copy(&chase_cam->camera.transform.position, &target->position);
    switch(chase_cam->mode) {
        case(CAMERA_MODE_SIDE):
            chase_cam->camera.transform.position.y += chase_cam->target_follow_height;
            chase_cam->camera.transform.position.x += chase_cam->target_follow_dist;
            break;
        case(CAMERA_MODE_TOP):
            chase_cam->camera.transform.position.z += chase_cam->target_follow_height;
            chase_cam->camera.transform.position.y += chase_cam->target_follow_dist;
            break;
        case(CAMERA_MODE_CHASE): /* fall through */
        default:
            chase_cam->camera.transform.position.y += chase_cam->target_follow_height;
            chase_cam->camera.transform.position.z += chase_cam->target_follow_dist;
            break;
    }
    
}

void chase_camera_get_forward(ChaseCamera* chase_cam, Vec3* out) {
    fw64_transform_forward(&chase_cam->camera.transform, out);
    out->y = 0.0f;
    vec3_normalize(out);
}
