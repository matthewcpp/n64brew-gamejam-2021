#pragma once

#include "framework64/animation_controller.h"
#include "framework64/animation_data.h"
#include "framework64/engine.h"
#include "framework64/node.h"
#include "framework64/scene.h"

#include "sparkle.h"
#include "shadow.h"

#define PLAYER_DEFAULT_ACCELERATION 50.0f
#define PLAYER_DEFAULT_DECELERATION 50.0f
#define PLAYER_DEFAULT_MAX_SPEED 30.0f

#define PLAYER_DEFAULT_ROTATION_SPEED 270.0f

#define PLAYER_DEFAULT_JUMP_VELOCITY 25.0f
#define PLAYER_DEFAULT_GRAVITY (-50.0f)

#define PLAYER_STICK_THRESHOLD 0.15f

#define PLAYER_DEFAULT_HEIGHT 10.0f
#define PLAYER_DEFAULT_RADIUS 3.0f;

typedef enum {
    PLAYER_STATE_ON_GROUND,
    PLAYER_STATE_FALLING
} PlayerState;

typedef struct {
    fw64Node node;
    fw64Collider collider;
    fw64Engine* engine;
    fw64Scene* scene;

    fw64AnimationData* animation_data;
    fw64AnimationController animation_controller;

    int mesh_index;

    Vec3 previous_position;

    float jump_impulse;
    float air_velocity;
    float gravity;

    float height;
    float radius;

    float speed;
    float acceleration;
    float deceleration;
    float dash_speed;
    float max_speed;

    float rotation;
    float rotation_speed;
    PlayerState state;

    int double_jumps;
    int dashes;
    int is_dashing; //just to die for
    int is_rolling; //they hating
    float roll_timer, roll_timer_max;
    Vec3 roll_direction;
    float roll_height;
    int process_input;
    int controller_num;

    Sparkle sparkle;
    Shadow shadow;
    fw64Node* ground_node;
} Player;

#ifdef __cplusplus
extern "C" {
#endif

void player_init(Player* player, fw64Engine* engine, fw64Scene* scene);
void player_update(Player* player);
void player_draw(Player* player);

void player_reset(Player* player);
void player_reset_at_position(Player* player, Vec3* position);
void player_calculate_size(Player* player);
void player_switch_mesh(Player* player);

#ifdef __cplusplus
}
#endif