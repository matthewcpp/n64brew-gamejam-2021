#include "chase_camera.h"
#include "layermap.h"
#include <framework64/n64/controller_button.h>

#include <stddef.h>

#define CAMERA_FOLLOW_DIST 30.0f
#define CAMERA_FOLLOW_HEIGHT 12.0f
#define CAMERA_FORWARD_TARGET_DIST 0.0f
#define CAMERA_FORWARD_TARGET_HEIGHT 0.0f
#define CAMERA_ADJUST_SPEED 40.0f
#define CAMERA_COLLISION_RADIUS 1.0f
#define CAMERA_PROJ_MULT_H 0.27614f //magic projection math (assumes 45 degree vert fov and 4:3 aspect ratio)
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
    chase_cam->last_mode = CAMERA_MODE_CHASE;
    chase_cam->mode_changed = 0;
    chase_cam->mode_transition = 0;
    chase_cam->target_transition = 0;
    chase_cam->current_collision_distance_adjustment = 0.0f;
}

static void chase_camera_update_position(ChaseCamera* chase_cam) {   
    //only update in the right camera mode with good data
    if (!chase_cam->target) return;
    if (!chase_cam->scene)  return;
    if (chase_cam->mode == CAMERA_MODE_MANUAL) return;

    fw64_transform_forward(chase_cam->target, &chase_cam->player_forward_dir);

    //setup basic variables
    Vec3 vec_up = {0.0f, 1.0f, 0.0f};
    Vec3 camera_pos, camera_target;
    Vec3 vec_camera_settings_offset;
    vec3_zero(&vec_camera_settings_offset);     
    vec3_copy(&camera_pos, &chase_cam->camera.transform.position);
    vec3_copy(&camera_target, &chase_cam->last_target_position);

    float* camera_target_left_right;
    float* camera_target_forward_back;
    float* camera_target_up_down;

    float player_left_right;
    float player_forward_back;
    float player_up_down;

    float  left_right_bounding;
    float  forward_back_bounding;
    float  up_down_bounding;

    float* camera_offset_back;
    float* camera_offset_up;

    //add target position offset
    
    if(chase_cam->mode == CAMERA_MODE_SIDE && !(chase_cam->mode_transition || chase_cam->mode_changed)) {
        if(fabs(chase_cam->target_forward_dist - chase_cam->player_forward_dir.z) > fabs(chase_cam->target_forward_dist) + 0.75f) {
            chase_cam->target_forward_dist *= -1.0f;
            chase_cam->target_transition = 1;
        }
    }

    //this is really dumb, but it sets the axes correctly for the 3 different camera orientations
    //then we use the same conditions to translate the camera correctly for all orientations
    //todo: use actual math to just make everything relative to the camera plane
    switch(chase_cam->mode) {
        case(CAMERA_MODE_TOP):
            camera_offset_back         = &vec_camera_settings_offset.y;
            camera_offset_up           = &vec_camera_settings_offset.z;

            camera_target_left_right   = &camera_target.x;
            player_left_right          =  chase_cam->target->position.x;
            left_right_bounding        =  chase_cam->target_bounding_radius;

            camera_target_forward_back = &camera_target.y;
            player_forward_back        =  chase_cam->target->position.y + chase_cam->target_forward_dist;
            forward_back_bounding      =  chase_cam->target_bounding_height;

            camera_target_up_down      = &camera_target.z;
            player_up_down             =  chase_cam->target->position.z + chase_cam->target_forward_height;
            up_down_bounding           =  chase_cam->target_bounding_height;
            break;
        case(CAMERA_MODE_SIDE):
            camera_offset_back         = &vec_camera_settings_offset.x;
            camera_offset_up           = &vec_camera_settings_offset.y;

            camera_target_left_right   = &camera_target.z;
            player_left_right          =  chase_cam->target->position.z + (chase_cam->target_forward_dist * (1.0f - (chase_cam->current_collision_distance_adjustment / chase_cam->target_follow_dist)));
            left_right_bounding        =  chase_cam->target_bounding_height;

            camera_target_forward_back = &camera_target.x;
            player_forward_back        =  chase_cam->target->position.x;
            forward_back_bounding      =  chase_cam->target_bounding_radius;

            camera_target_up_down      = &camera_target.y;
            player_up_down             =  chase_cam->target->position.y + chase_cam->target_forward_height;
            up_down_bounding           =  chase_cam->target_bounding_height;
            break;
        case(CAMERA_MODE_CHASE): /* fall through */
        case(CAMERA_MODE_CHASE_GROUNDED): /* fall through */
        default:
            camera_offset_back         = &vec_camera_settings_offset.z;
            camera_offset_up           = &vec_camera_settings_offset.y;

            camera_target_left_right   = &camera_target.x;
            player_left_right          =  chase_cam->target->position.x;
            left_right_bounding        =  chase_cam->target_bounding_radius;

            camera_target_forward_back = &camera_target.z;
            player_forward_back        =  chase_cam->target->position.z + chase_cam->target_forward_dist;
            forward_back_bounding      =  chase_cam->target_bounding_height;

            camera_target_up_down      = &camera_target.y;
            player_up_down             =  chase_cam->target->position.y + chase_cam->target_forward_height;
            up_down_bounding           =  chase_cam->target_bounding_height;
            break;
    }

    //translate left or right
    if(*camera_target_left_right < player_left_right) {
        if(player_left_right - *camera_target_left_right > left_right_bounding) {
            *camera_target_left_right += (player_left_right - *camera_target_left_right - left_right_bounding);
        }
    }
    else {
        if(*camera_target_left_right - player_left_right > left_right_bounding) {
            *camera_target_left_right -= (*camera_target_left_right - player_left_right - left_right_bounding);
        }
    }

    //translate forward or backward
    if(*camera_target_forward_back < player_forward_back) {
        if(player_forward_back - *camera_target_forward_back > forward_back_bounding) {
            *camera_target_forward_back += (player_forward_back - *camera_target_forward_back - forward_back_bounding);
        }
    }
    else {
        if(*camera_target_forward_back - player_forward_back > forward_back_bounding) {
            *camera_target_forward_back -= (*camera_target_forward_back - player_forward_back - forward_back_bounding);
        }
    }

    //translate up or down
    float y_val = (up_down_bounding / 2) + player_up_down;
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

    if(chase_cam->target_transition) {
        float target_pos_error_dist = vec3_distance(&camera_target, &chase_cam->last_target_position);
        float adj_speed = chase_cam->camera_adjust_speed * chase_cam->engine->time->time_delta;
        if(target_pos_error_dist < adj_speed) {
            chase_cam->target_transition = 0;
        }
        else {
            Vec3 ideal_target_position, target_pos_error_vec;
            vec3_copy(&ideal_target_position, &camera_target);
            vec3_subtract(&target_pos_error_vec, &ideal_target_position, &chase_cam->last_target_position);
            vec3_normalize(&target_pos_error_vec);
            vec3_scale(&target_pos_error_vec, &target_pos_error_vec, adj_speed);
            vec3_add(&camera_target, &chase_cam->last_target_position, &target_pos_error_vec);
        }
    }
    
    //offset camera from target
    vec3_zero(&vec_camera_settings_offset);
    *camera_offset_up   = chase_cam->target_follow_height;
    *camera_offset_back = chase_cam->target_follow_dist;

    if(chase_cam->mode_changed) {
        chase_cam->mode_changed = 0;
        if(chase_cam->last_mode != chase_cam->mode)
        {
            chase_cam->mode_transition = 1;        
            chase_cam->previous_mode_position = chase_cam->camera.transform.position;
            
            if(chase_cam->mode == CAMERA_MODE_CHASE_GROUNDED) {
                Vec3 temp_vec;
                vec3_set(&temp_vec, 0.0f, chase_cam->target_follow_height - chase_cam->target_forward_height, chase_cam->target_follow_dist);
                vec3_add(&chase_cam->new_mode_ideal_position, &camera_target, &temp_vec);
            }
            else {
                vec3_add(&chase_cam->new_mode_ideal_position, &camera_target, &vec_camera_settings_offset);
            }            
            chase_cam->mode_transition_total_distance = vec3_distance(&chase_cam->previous_mode_position, &chase_cam->new_mode_ideal_position);
            chase_cam->mode_transition_current_distance = 0.0f;
        }
    }
    else if(chase_cam->mode_transition) {
        vec3_add(&chase_cam->new_mode_ideal_position, &camera_target, &vec_camera_settings_offset);
        chase_cam->mode_transition_total_distance = vec3_distance(&chase_cam->previous_mode_position, &chase_cam->new_mode_ideal_position);

        chase_cam->mode_transition_current_distance += chase_cam->camera_adjust_speed * chase_cam->engine->time->time_delta;

        if(chase_cam->mode_transition_current_distance >= chase_cam->mode_transition_total_distance) {
            vec3_copy(&camera_pos, &chase_cam->new_mode_ideal_position);
            chase_cam->mode_transition = 0;
        }
        else {
            float t = chase_cam->mode_transition_current_distance / chase_cam->mode_transition_total_distance;
            vec3_smoothstep(&camera_pos, &chase_cam->previous_mode_position, &chase_cam->new_mode_ideal_position, t);
        }        
    }
    else if(chase_cam->mode==CAMERA_MODE_CHASE_GROUNDED) {
        Vec3 temp_vec;
        vec3_set(&temp_vec, 0.0f, 0.0f, chase_cam->target_follow_dist);
        vec3_add(&camera_pos, &camera_target, &temp_vec);

        fw64RaycastHit gnd_ray_hit;
        vec3_set(&temp_vec, 0.0f, -1.0f, 0.0f);
        if(fw64_scene_raycast(chase_cam->scene, &camera_pos, &temp_vec, NODE_LAYER_GROUND, &gnd_ray_hit)){
            vec3_copy(&camera_pos, &gnd_ray_hit.point);
            vec3_set(&temp_vec, 0.0f, chase_cam->target_follow_height, 0.0f);
            vec3_add(&camera_pos, &camera_pos, &temp_vec);
        }
        else{
            vec3_copy(&camera_pos, &chase_cam->camera.transform.position);
            vec3_set(&temp_vec, camera_target.x - chase_cam->last_target_position.x, 0.0f, camera_target.z - chase_cam->last_target_position.z);
            vec3_add(&camera_pos, &camera_pos, &temp_vec);
        }
    }
    else {
        vec3_add(&camera_pos, &camera_target, &vec_camera_settings_offset);
    }

    float camera_total_dist = vec3_distance(&camera_target, &camera_pos);
    float camera_ideal_distance = camera_total_dist;

    if(!(chase_cam->mode_changed || chase_cam->mode_transition) && (chase_cam->mode != CAMERA_MODE_CHASE_GROUNDED)) //dirty hacks. remove later
    {
        //detect scene geometry blocking line of sight and move forward if needed
        fw64RaycastHit ray_hit;
        Vec3 ray_dir;
        vec3_subtract(&ray_dir, &camera_pos, &camera_target);
        vec3_normalize(&ray_dir);
        fw64_scene_raycast(chase_cam->scene, &camera_target, &ray_dir, ~(NODE_LAYER_GROUND), &ray_hit);
        
        if(ray_hit.distance < camera_total_dist) {
            vec3_subtract(&ray_dir, &ray_hit.point, &camera_pos);
            vec3_normalize(&ray_dir);
            vec3_scale(&ray_dir, &ray_dir, (camera_total_dist - ray_hit.distance) + chase_cam->collision_radius);
            vec3_add(&camera_pos, &camera_pos, &ray_dir);
        }
    }

    //persist work to actual camera state
    vec3_copy(&chase_cam->camera.transform.position, &camera_pos);
    vec3_copy(&chase_cam->last_target_position, &camera_target);

    //update screen space player boundaries
    camera_total_dist = vec3_distance(&chase_cam->last_target_position, &chase_cam->camera.transform.position);
    chase_cam->current_collision_distance_adjustment = camera_ideal_distance - camera_total_dist;
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

void chase_camera_set_mode(ChaseCamera* chase_cam, CameraMode new_mode, float new_follow_dist, float new_follow_height, float new_forward_dist, float new_forward_height) {
    chase_cam->last_mode = chase_cam->mode;
    chase_cam->mode = new_mode;
    chase_cam->target_follow_dist = new_follow_dist;
    chase_cam->target_follow_height = new_follow_height;
    chase_cam->target_forward_dist = new_forward_dist;
    chase_cam->target_forward_height = new_forward_height;
    chase_cam->mode_changed = 1;
}
