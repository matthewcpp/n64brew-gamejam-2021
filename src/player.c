#include "player.h"

#include "assets.h"

#include "framework64/n64/controller_button.h"

#include <stdio.h>

static void process_input(Player* player);
static void update_position(Player* player);

void player_init(Player* player, fw64Engine* engine, fw64Scene* scene, Vec3* position) {
    player->engine = engine;
    player->scene = scene;

    player->jump_impulse = PLAYER_DEFAULT_JUMP_VELOCITY;
    player->gravity = PLAYER_DEFAULT_GRAVITY;
    player->dash_speed = PLAYER_DEFAULT_MAX_SPEED * 3.5;
    player->max_speed = PLAYER_DEFAULT_MAX_SPEED;
    player->acceleration = PLAYER_DEFAULT_ACCELERATION;
    player->deceleration = PLAYER_DEFAULT_DECELERATION;

    player->process_input = 1;
    player->controller_num = 0;

    player->height = PLAYER_DEFAULT_HEIGHT;
    player->radius = PLAYER_DEFAULT_RADIUS;

    player->double_jumps = 0;
    player->dashes = 0;
    player->is_dashing = 0;

    player->mesh_index = 0;
    player->meshes[0] = fw64_mesh_load(engine->assets, FW64_ASSET_mesh_penguin);
    player->meshes[1] = fw64_mesh_load(engine->assets, FW64_ASSET_mesh_toad);

    fw64_node_init(&player->node);
    fw64_node_set_mesh(&player->node,  player->meshes[player->mesh_index]);
    fw64_node_set_box_collider(&player->node, &player->collider);

    player_reset(player, position);

    sparkle_init(&player->sparkle, engine);
}

void player_reset(Player* player, Vec3* position) {
    player->speed = 0.0f;
    player->rotation = 0.0f;
    player->state = PLAYER_STATE_ON_GROUND;
    player->air_velocity = 0.0f;

    quat_ident(&player->node.transform.rotation);

    if (position) {
        player->node.transform.position = *position;
        fw64_node_update(&player->node);
    }
}

void player_update(Player* player) {
    if (player->process_input)
        process_input(player);

    update_position(player);

    player->sparkle.node.transform.position = player->node.transform.position;
    sparkle_update(&player->sparkle);

    if (player->sparkle.is_active) {
        float switch_time = SPARKLE_DURARION / 2.0f;

        if (player->sparkle.prev_time < switch_time && player->sparkle.current_time >= switch_time){
            player_switch_mesh(player);

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
}

void process_input(Player* player) {
    Vec2 stick;
    fw64_input_stick(player->engine->input, 0, &stick);

    if(player->is_dashing)
    {
        float decel = player->deceleration * player->engine->time->time_delta;
        player->speed = fmaxf((player->speed - decel), 0.0f);
        if(player->speed <= player->max_speed)
            player->is_dashing = 0;
    }
    else
    {
        if (stick.x >= PLAYER_STICK_THRESHOLD || stick.x <= -PLAYER_STICK_THRESHOLD) {
            float rotation_delta = PLAYER_DEFAULT_ROTATION_SPEED * player->engine->time->time_delta;

            if (stick.x >= PLAYER_STICK_THRESHOLD) {
                player->rotation -= rotation_delta;
            }
            else if (stick.x <= -PLAYER_STICK_THRESHOLD) {
                player->rotation += rotation_delta;
            }
            quat_set_axis_angle(&player->node.transform.rotation, 0, 1, 0, player->rotation * ((float)M_PI / 180.0f));
        }

        if (stick.y >= PLAYER_STICK_THRESHOLD ) {
            player->speed = fmaxf(player->max_speed * 0.5f, fminf(player->speed + player->acceleration * player->engine->time->time_delta, player->max_speed));
        }
        else if (stick.y <= -PLAYER_STICK_THRESHOLD) {
            player->speed = fminf(fmaxf(player->speed - player->acceleration * player->engine->time->time_delta, -player->max_speed), player->max_speed * -0.5f);
        }
        else {
            float decel = player->deceleration * player->engine->time->time_delta;
            if (player->speed > 0.0f)
                player->speed = fminf(fmaxf(player->speed - decel, 0.0f), player->max_speed * 0.5f);
            else if (player->speed < 0.0f)
                player->speed = fmaxf(fminf(player->speed + decel, 0.0f), player->max_speed * -0.5f);
        }
    }
    
    if (fw64_input_button_pressed(player->engine->input, player->controller_num, FW64_N64_CONTROLLER_BUTTON_A)) {
        if (player->state == PLAYER_STATE_ON_GROUND) {
            player->air_velocity = player->jump_impulse;
        }
        else if (player->double_jumps > 0) {
            player->air_velocity = player->jump_impulse * (0.85f * player->double_jumps);
            player->double_jumps -= 1;
        }
    }
    else if(fw64_input_button_released(player->engine->input, player->controller_num, FW64_N64_CONTROLLER_BUTTON_A)) {
        if (player->state == PLAYER_STATE_FALLING) {
            player->air_velocity = fminf(fmaxf(player->air_velocity - 2.0f, 0.0f), player->air_velocity);
        }
    }

    if (fw64_input_button_pressed(player->engine->input, player->controller_num, FW64_N64_CONTROLLER_BUTTON_B) && player->dashes > 0) {
        player->speed = player->dash_speed;
        player->is_dashing = 1;
    }

    if (fw64_input_button_pressed(player->engine->input, player->controller_num, FW64_N64_CONTROLLER_BUTTON_Z) && !player->sparkle.is_active) {
        sparkle_start(&player->sparkle);
    }
        
}

static Vec3 calculate_movement_vector(Player* player) {
    float time_delta = player->engine->time->time_delta;

    Vec3 movement;
    fw64_transform_forward(&player->node.transform, &movement);
    vec3_scale(&movement, &movement, player->speed * time_delta);

    movement.y += (player->air_velocity * time_delta) + (player->gravity * time_delta * time_delta / 2.0f);
    player->air_velocity += player->gravity * time_delta;

    return movement;
}

#define LAYER_DEFAULT 1U
#define LAYER_GROUND 2U
#define LAYER_WALL 4U

void update_position(Player* player) {
    Vec3* position = &player->node.transform.position;

    Vec3 movement = calculate_movement_vector(player);
    vec3_add(position, position, &movement);

    PlayerState new_state = PLAYER_STATE_FALLING;
    float height_radius = player->height / 2.0f;
    Vec3 query_center = *position;
    query_center.y += height_radius;

    fw64OverlapSphereQueryResult result;
    if (fw64_scene_overlap_sphere(player->scene, &query_center, height_radius, 0xFFFFFFFF, &result)) {
        for (int i = 0; i < result.count; i++) {
            fw64OverlapSphereResult *hit = result.results + i;

            Vec3 direction;
            vec3_subtract(&direction, &query_center, &hit->point);
            vec3_normalize(&direction);

            int is_grounded = (hit->node->layer_mask & LAYER_GROUND) || direction.y > 0.9f;

            // ground
            if (is_grounded && player->air_velocity <= 0.0f) {
                new_state = PLAYER_STATE_ON_GROUND;
                
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

    fw64_node_update(&player->node);
}

void player_draw(Player* player) {
    fw64_renderer_draw_static_mesh(player->engine->renderer, &player->node.transform, player->node.mesh);
    sparkle_draw(&player->sparkle);
}

void player_calculate_size(Player* player) {
    Vec3 extents;
    box_extents(&player->node.collider->bounding, &extents);
    player->height = extents.y * 2.0f;
    player->radius = extents.x > extents.z ? extents.x : extents.z;

    vec3_set_all(&player->sparkle.node.transform.scale, 5);
}

void player_switch_mesh(Player* player) {
    player->mesh_index = player->mesh_index == 0 ? 1 : 0;
    fw64_node_set_mesh(&player->node, player->meshes[player->mesh_index]);
    fw64_node_update(&player->node);
    player_calculate_size(player);
}