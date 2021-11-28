#pragma once

#include "player.h"

#define STATIC_FIRE_OBSTACLE_NODE_COUNT 4

typedef enum  {
    STATIC_FIRE_OBSTACLE_ROLL,
    STATIC_FIRE_OBSTACLE_JUMP
} StaticFireObstacleType;

typedef struct {
    StaticFireObstacleType type;
    Player* player;
    fw64Node nodes[STATIC_FIRE_OBSTACLE_NODE_COUNT];
    Box box;
    int active;
} StaticFireObstacle;

#ifdef __cplusplus
extern "C" {
#endif

void static_fire_obstacle_init(StaticFireObstacle* obstacle, StaticFireObstacleType type, fw64Node* reference, Player* player, fw64Mesh* flame_mesh);
int static_fire_obstacle_update(StaticFireObstacle* obstacle, float time_delta);
void static_fire_obstacle_draw(StaticFireObstacle* obstacle, fw64Renderer* renderer);

#ifdef __cplusplus
}
#endif