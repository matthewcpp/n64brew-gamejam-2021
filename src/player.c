#include "player.h"

#include "assets.h"
#include "sound_bank_sound_effects.h"
#include "catherine_animation.h"

#include "layermap.h"

#include "framework64/n64/controller_button.h"
#include "framework64/matrix.h"

#include <stdio.h>
#include <string.h>

static void process_input(Player* player);
static void update_controller_num(Player* player);
static void update_position(Player* player);

static void player_tweak_root_animation_rotation(Player* player);

void player_init(Player* player, fw64Engine* engine, fw64Scene* scene, fw64Allocator* allocator) {
    player->engine = engine;
    player->scene = scene;
    player->settings = NULL;

    player->jump_impulse = PLAYER_DEFAULT_JUMP_VELOCITY;
    player->gravity = PLAYER_DEFAULT_GRAVITY;
    player->dash_speed = PLAYER_DEFAULT_MAX_SPEED * 3.0f;
    player->max_speed = PLAYER_DEFAULT_MAX_SPEED;
    player->acceleration = PLAYER_DEFAULT_ACCELERATION;
    player->deceleration = PLAYER_DEFAULT_DECELERATION;

    player->ground_node = NULL;

    player->process_input = 1;
    player->controller_num = 0;

    player->height = PLAYER_DEFAULT_HEIGHT;
    player->radius = PLAYER_DEFAULT_RADIUS;

    player->roll_timer_max = PLAYER_DEFAULT_ROLL_TIME;

    player->mesh_index = 0;
    player->deaths = 0;

    fw64_node_init(&player->node);
    fw64Mesh* player_mesh = fw64_mesh_load(engine->assets, FW64_ASSET_mesh_catherine, allocator);
    player_palette_init(&player->palette, engine, player_mesh, allocator);
    player->animation_data = fw64_animation_data_load(engine->assets, FW64_ASSET_animation_data_catherine, allocator);
    
    fw64_animation_controller_init(&player->animation_controller, player->animation_data, -1, allocator);
    player_tweak_root_animation_rotation(player);
    fw64_node_set_mesh(&player->node,  player_mesh);
    fw64_node_set_box_collider(&player->node, &player->collider);

    vec3_set(&player->camera_forward, 0.0f, 0.0f, -1.0f);

    player_reset(player);

    sparkle_init(&player->sparkle, engine, allocator);
    shadow_init(&player->shadow, engine, NULL, &player->node.transform, allocator);
}


void player_uninit(Player* player, fw64Allocator* allocator) {
    fw64_mesh_delete(player->engine->assets, player->node.mesh, allocator);
    fw64_animation_data_delete(player->animation_data, allocator);
    fw64_animation_controller_uninit(&player->animation_controller, allocator);

    shadow_uninit(&player->shadow, allocator);
    sparkle_uninit(&player->sparkle, allocator);
}

void player_reset(Player* player) {
    player->speed = 0.0f;
    player->rotation = 0.0f;
    player->state = PLAYER_STATE_ON_GROUND;
    player->air_velocity = 0.0f;

    // reset player abilities
    player->double_jumps = 0;
    player->dashes = 0;
    player->is_dashing = 0;
    player->is_rolling = 0;
    player->roll_timer = 0.0f;

    quat_ident(&player->node.transform.rotation);

    fw64_node_update(&player->node);

    fw64_animation_controller_set_animation(&player->animation_controller, catherine_animation_Idle);
    player->animation_controller.speed = 0.25f;
    fw64_animation_controller_play(&player->animation_controller);
}

void player_reset_at_position(Player* player, Vec3* position) {
    player->node.transform.position = *position;
    player->previous_position = *position;
    
    player_reset(player);
}

void player_set_scene(Player* player, fw64Scene* scene) {
    player->scene = scene;
    player->shadow.scene = scene;
}

static void update_player_swap(Player* player) {
    float switch_time = SPARKLE_DURATION / 2.0f;

    if (player->sparkle.prev_time < switch_time && player->sparkle.current_time >= switch_time){
        player->mesh_index = player->mesh_index == 0 ? 1 : 0;

        if (player->mesh_index == 0) 
            player_palette_activate_primary(&player->palette);
        else 
            player_palette_activate_secondary(&player->palette);


        if(player->mesh_index == 0) {
            player->dashes = 1;
            player->double_jumps = 0;
        }
        else {
            player->dashes = 0;
            player->double_jumps = 1;
        }
    }
}

static void update_roll(Player* player) {
        Quat rotation;
        quat_from_euler(&rotation, player->engine->time->time_delta * (-360.0f / player->roll_timer_max), 0, 0);

        float ax,ay,az,aw,bx,by,bz,bw,rx,ry,rz,rw;

        ax = player->node.transform.rotation.x;
        ay = player->node.transform.rotation.y;
        az = player->node.transform.rotation.z;
        aw = player->node.transform.rotation.w;

        bx = rotation.x;
        by = rotation.y;
        bz = rotation.z;
        bw = rotation.w;

        rx = ax * bw + aw * bx + ay * bz - az * by;
        ry = ay * bw + aw * by + az * bx - ax * bz;
        rz = az * bw + aw * bz + ax * by - ay * bx;
        rw = aw * bw - ax * bx - ay * by - az * bz;

        quat_set(&player->node.transform.rotation, rx, ry, rz, rw);
}

static void update_animation(Player* player) {
    if (player->speed == 0.0f) {
        if (player->animation_controller.current_animation != fw64_animation_data_get_animation(player->animation_data, catherine_animation_Idle)) {
            fw64_animation_controller_set_animation(&player->animation_controller, catherine_animation_Idle);
            player->animation_controller.speed = 0.25f;
        }
    }
    else {
        if (player->animation_controller.current_animation != fw64_animation_data_get_animation(player->animation_data, catherine_animation_Run)) {
            fw64_animation_controller_set_animation(&player->animation_controller, catherine_animation_Run);
            //player->animation_controller.speed = 1.0f;

        }
        else {
            player->animation_controller.speed = player->speed / player->max_speed;
        }
    }
        

    fw64_animation_controller_update(&player->animation_controller, player->engine->time->time_delta);
}

void player_update(Player* player) {
    if (player->process_input)
        process_input(player);

    update_position(player);

    player->sparkle.node.transform.position = player->node.transform.position;
    sparkle_update(&player->sparkle);
    
    shadow_update(&player->shadow);

    if(player->is_rolling) {
        update_roll(player);
    }    
    
    if (player->sparkle.is_active) {
        update_player_swap(player);
    }

    update_animation(player);
}

static void player_update_stick(Player* player) {
    Vec2 stick;
    fw64_input_stick(player->engine->input, player->controller_num, &stick);

    if (stick.y >= PLAYER_STICK_THRESHOLD || stick.y <= -PLAYER_STICK_THRESHOLD ||
        stick.x >= PLAYER_STICK_THRESHOLD || stick.x <= -PLAYER_STICK_THRESHOLD) { //joystick y axis, move forward/backward
        
        player->rotation  = atan2(-player->camera_forward.z, player->camera_forward.x) * (180.0f / M_PI) - 90.0f;
        player->rotation += atan2(stick.y, stick.x) * (180.0f / M_PI) - 90.0f;

        quat_set_axis_angle(&player->node.transform.rotation, 0, 1, 0, player->rotation * ((float)M_PI / 180.0f));
        Vec3 vec_temp_zero, vec_temp_stick; 
        vec3_set(&vec_temp_stick, stick.x, stick.y, 0.0f);
        vec3_zero(&vec_temp_zero);
        //player->speed = fmaxf(player->max_speed * 0.5f, fminf(player->speed + player->acceleration * player->engine->time->time_delta, player->max_speed));
        player->speed = fminf(player->max_speed * vec3_distance(&vec_temp_zero, &vec_temp_stick), player->max_speed);
    }
    else { //apply friction
        float decel = player->deceleration * player->engine->time->time_delta;
        if (player->speed > 0.0f)
            player->speed = fminf(fmaxf(player->speed - decel, 0.0f), player->max_speed * 0.5f);
    }
}

void update_controller_num(Player* player) {
    if(player->settings == NULL) {
        player->controller_num = 0; //no settings to go off of. use 0 as sane default
        return;
    }

    switch(player->settings->control_mode)
    {
        case CONTROL_MODE_SINGLE: {
            //single player mode. use the current controller if it's detected.
            //if not, use the lowest detected controller number.
            //if not controllers detected, default to 0.
            if(fw64_input_controller_is_connected(player->engine->input, player->controller_num))
                return;
            for(int i = 0; i < 4; i++) {
                if(fw64_input_controller_is_connected(player->engine->input, i)) {
                    player->controller_num = i;
                    return;
                }
            }
            //todo: if we get to this, we don't have enough controllers plugged in.
            //      should pause and say "controller disconnected" or something
            player->controller_num = 0;
            return;
        }
        case CONTROL_MODE_MULTI_TIMER: {
            //multiplayer mode
            //requires at least 2 controllers
            //active controller changes on a fixed timer
            static float dt = 0.0f;
            dt += player->engine->time->time_delta;
            if(dt >= player->settings->control_transfer_time)
            {
                dt -= player->settings->control_transfer_time;

                for(int i = 1; i < 4; i++) {
                    if(fw64_input_controller_is_connected(player->engine->input, ((player->controller_num + i) % 4))) {
                        player->controller_num = (player->controller_num + i) % 4;
                        return;
                    }
                }

                //todo: if we get to this, we don't have enough controllers plugged in.
                //      should pause and say "controller disconnected" or something
                player->controller_num = (player->controller_num + 1) % 4;
            }
            return;
        }
        case CONTROL_MODE_MULTI_ONPRESS: {
            //multiplayer mode
            //requires at least 2 controllers
            //active controller changes when Z is pressed

            if(player->sparkle.is_active)
                return;

            //if Z pressed by active controller, move to next available controller
            if(fw64_input_button_pressed(player->engine->input, player->controller_num, FW64_N64_CONTROLLER_BUTTON_Z)) {
                for(int i = 1; i < 4; i++) {
                    if(fw64_input_controller_is_connected(player->engine->input, ((player->controller_num + i) % 4))) {
                        player->controller_num = (player->controller_num + i) % 4;
                        return;
                    }
                }
            }
            else { //else if any other controller presses Z, switch to THAT controller
                for(int i = 1; i < 4; i++) {
                    if(fw64_input_controller_is_connected(player->engine->input, ((player->controller_num + i) % 4))) {
                        if(fw64_input_button_pressed(player->engine->input, ((player->controller_num + i) % 4), FW64_N64_CONTROLLER_BUTTON_Z)) {
                            player->controller_num = (player->controller_num + i) % 4;
                            return;
                        }
                    }
                }
            }

            return;
        }
        default: { //something went very wrong. just default to 0
            player->controller_num = 0;
            return;
        }
    }
}

void process_input(Player* player) {

    int prev_controller_num = player->controller_num;
    update_controller_num(player);

    if(player->is_dashing) //dashing mechanic
    {
        if(fw64_input_button_released(player->engine->input, player->controller_num, FW64_N64_CONTROLLER_BUTTON_B)) {
            if(player->speed > player->max_speed)
                player->speed = fmaxf(player->speed * 0.75f, player->max_speed); //cap speed on release, also cancels out of dashing
        }
        else
        {
            float decel = player->state == PLAYER_STATE_ON_GROUND ? player->deceleration : player->deceleration * 2.5f;
            decel *= player->engine->time->time_delta;
            player->speed = fmaxf((player->speed - decel), player->max_speed);
        }
        
        if(player->speed <= player->max_speed)
            player->is_dashing = 0;
    }
    else if(player->is_rolling) //rolling mechanic
    {
        player->speed = player->max_speed;

        player->roll_timer -= player->engine->time->time_delta;

        if(player->roll_timer <= 0.0f)
        {
            player->roll_timer = 0.0f;
            player->is_rolling = 0;
        }
    }
    else
    {
        player_update_stick(player);

        if (fw64_input_button_pressed(player->engine->input, player->controller_num, FW64_N64_CONTROLLER_BUTTON_A)) { //jump
            if (player->state == PLAYER_STATE_ON_GROUND) {
                player->air_velocity = player->jump_impulse;
            }
            else if (player->double_jumps > 0) { //double jump
                player->air_velocity = player->jump_impulse;// * (0.85f * player->double_jumps);
                player->double_jumps -= 1;
            }
        }
        else if(fw64_input_button_released(player->engine->input, player->controller_num, FW64_N64_CONTROLLER_BUTTON_A)) { //shorten jump on release
            if (player->state == PLAYER_STATE_FALLING && player->air_velocity > 0) {
                player->air_velocity *= 0.5f;//player->air_velocity = fminf(fmaxf(player->air_velocity - 2.0f, 0.0f), player->air_velocity);
            }
        }

        if (fw64_input_button_pressed(player->engine->input, player->controller_num, FW64_N64_CONTROLLER_BUTTON_B)) { //dash or roll
            if(player->dashes > 0) {
                fw64_audio_play_sound(player->engine->audio, sound_bank_sound_effects_dash);
                player->speed = player->dash_speed;
                player->is_dashing = 1;
                player->air_velocity = 0;
                player->dashes -= 1;
            }
            else if(player->state == PLAYER_STATE_ON_GROUND && player->mesh_index == 1 && !player->is_rolling) //roll
            {
                player->is_rolling = 1;
                player->roll_timer = player->roll_timer_max; //roll time in seconds
                fw64_transform_forward(&player->node.transform, &player->roll_direction);
                player->roll_direction.y = 0;
            }
        }

        if (fw64_input_button_pressed(player->engine->input, player->controller_num, FW64_N64_CONTROLLER_BUTTON_Z) && !player->sparkle.is_active) { //swap character
            fw64_audio_play_sound(player->engine->audio, sound_bank_sound_effects_swap);
            sparkle_start(&player->sparkle);
        }
        if((prev_controller_num != player->controller_num) && player->settings->control_mode == CONTROL_MODE_MULTI_ONPRESS) {
                if (fw64_input_button_pressed(player->engine->input, prev_controller_num, FW64_N64_CONTROLLER_BUTTON_Z) && !player->sparkle.is_active) { //swap character
                fw64_audio_play_sound(player->engine->audio, sound_bank_sound_effects_swap);
                sparkle_start(&player->sparkle);
            }
        }
        
    }
}

static Vec3 calculate_movement_vector(Player* player) {
    float time_delta = player->engine->time->time_delta;

    Vec3 movement;
    if(player->is_rolling) {
        vec3_copy(&movement, &player->roll_direction);
    }
    else {
        fw64_transform_forward(&player->node.transform, &movement);
    }
    vec3_scale(&movement, &movement, player->speed * time_delta);

    if(!(player->is_dashing)) //cancel gravity while dashing
    {
        movement.y += (player->air_velocity * time_delta) + (player->gravity * time_delta * time_delta / 2.0f);
        player->air_velocity = fmaxf(-60.0,(player->air_velocity+(player->gravity * time_delta))); //pengy terminal velocity
    }

    return movement;
}

static void update_position_step(Player* player, Vec3* movement) {
    player->previous_position = player->node.transform.position;
    Vec3* position = &player->node.transform.position;

    vec3_add(position, position, movement);

    PlayerState new_state = PLAYER_STATE_FALLING;
    float height_radius = player->height / 2.0f;
    Vec3 query_center = *position;
    query_center.y += height_radius;

    player->ground_node = NULL;

    fw64OverlapSphereQueryResult result;
    if (fw64_scene_overlap_sphere(player->scene, &query_center, height_radius, 0xFFFFFFFF, &result)) {
        for (int i = 0; i < result.count; i++) {
            fw64OverlapSphereResult *hit = result.results + i;

            Vec3 direction;
            vec3_subtract(&direction, &query_center, &hit->point);
            vec3_normalize(&direction);

            Vec3 abs_dir = direction;
            abs_dir.x = fw64_fabsf(direction.x);
            abs_dir.y = fw64_fabsf(direction.y);
            abs_dir.z = fw64_fabsf(direction.z);
            
            int is_grounded = (abs_dir.y > abs_dir.x && abs_dir.y > abs_dir.z);

            // ground
            if (is_grounded && player->air_velocity <= 0.0f) {
                new_state = PLAYER_STATE_ON_GROUND;
                player->ground_node = hit->node;
                
                //reset our double jump or dash counter
                if(player->mesh_index == 1)
                    player->double_jumps = 1;
                else if(player->is_dashing == 0)
                    player->dashes = 1;
                
                player->air_velocity = 0;
                float hit_distance = vec3_distance(&query_center, &hit->point);
                vec3_add_and_scale(position, position, &direction, height_radius - hit_distance);
            } else { // walls
                float hit_distance = vec3_distance(&query_center, &hit->point);

                if (hit_distance < player->radius) {
                    vec3_add_and_scale(position, position, &direction, player->radius - hit_distance);
                }
            }
        }
    }

    player->state = new_state;
}

void update_position(Player* player) {
    Vec3 movement = calculate_movement_vector(player);

    if (player->is_dashing) {
        vec3_scale(&movement, &movement, 1.0f / 3.0f);
        for (int i = 0; i < 3; i++) {
            update_position_step(player, &movement);
        }
    }
    else  {
        update_position_step(player, &movement);
    }
    

    fw64_node_update(&player->node);
}

void player_draw(Player* player) {
    fw64_renderer_draw_animated_mesh(player->engine->renderer, player->node.mesh, &player->animation_controller, &player->node.transform);
    shadow_draw(&player->shadow);
    sparkle_draw(&player->sparkle);
}

void player_calculate_size(Player* player) {
    Vec3 extents;
    box_extents(&player->node.collider->bounding, &extents);
    player->height = extents.y * 2.0f;
    player->radius = extents.x > extents.z ? extents.x : extents.z;

    vec3_set_all(&player->sparkle.node.transform.scale, 5);
}

#ifdef PLATFORM_N64
#include <nusys.h>
#endif

// This is not ideal, however the model as authored is oriented looking down +z, however in framework64 forward is -z
// no luck fixing it in blender without messing up the rig so We apply a base rotation to the root of the joint hierarchy to fix this
void player_tweak_root_animation_rotation(Player* player) {
    float fix_matrix[16];
    Quat q;
    quat_set_axis_angle(&q, 0.0f, 1.0f, 0.0f, M_PI);
    matrix_from_quat(&fix_matrix[0], &q);

    fw64Matrix* root_matrix = player->animation_controller.matrices;

    #ifdef PLATFORM_N64
        guMtxF2L((float (*)[4])fix_matrix, root_matrix);
    #else
        memcpy(&root_matrix->m[0], fix_matrix, sizeof(float) * 16);
    #endif
}

void player_set_game_settings(Player* player, GameSettings* settings) {
    player->settings = settings;
}