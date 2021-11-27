#pragma once

#include "player.h"

#define STATIC_FIRE_OBSTACLE_NODE_COUNT 4

typedef struct {
    Player* player;
    fw64Node nodes[STATIC_FIRE_OBSTACLE_NODE_COUNT];
    Box box;
} StaticFireObstacle;

#ifdef __cplusplus
extern "C" {
#endif

void fire_rolling_obstacle_init(StaticFireObstacle* obstacle, fw64Node* reference, Player* player, fw64Mesh* flame_mesh);
int fire_rolling_obstacle_update(StaticFireObstacle* obstacle, float time_delta);

void fire_jumping_obstacle_init(StaticFireObstacle* obstacle, fw64Node* reference, Player* player, fw64Mesh* flame_mesh);
int fire_jumping_obstacle_update(StaticFireObstacle* obstacle, float time_delta);

void draw_static_fire_obstacles(fw64Renderer* renderer, StaticFireObstacle* obstacles, int count);

#ifdef __cplusplus
}
#endif