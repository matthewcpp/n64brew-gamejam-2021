#pragma once

#include "player.h"


#define FIRE_OBSTACLE_POOL_SIZE 10
typedef struct {
    fw64Node nodes[FIRE_OBSTACLE_POOL_SIZE];
    fw64Node* stack[FIRE_OBSTACLE_POOL_SIZE];
    int index;
} FireObstacleNodePool;

void fire_obstacle_node_pool_init(FireObstacleNodePool* pool, fw64Mesh* mesh);
fw64Node* fire_obstacle_node_pool_get(FireObstacleNodePool* pool);
void fire_obstacle_node_pool_return(FireObstacleNodePool* pool, fw64Node* node);


#define ROLL_OBSTACLE_NODE_COUNT 4

typedef struct {
    Player* player;
    fw64Node nodes[ROLL_OBSTACLE_NODE_COUNT];
    Box box;
} FireRollingObstacle;

#ifdef __cplusplus
extern "C" {
#endif

void fire_rolling_obstacle_init(FireRollingObstacle* obstacle, fw64Node* reference, Player* player, fw64Mesh* flame_mesh);
int fire_rolling_obstacle_update(FireRollingObstacle* obstacle, float time_delta);
void fire_rolling_obstacle_draw(FireRollingObstacle* obstacle, fw64Renderer* renderer);

typedef struct {
    Player* player;
    fw64Node nodes[ROLL_OBSTACLE_NODE_COUNT];
    Box box;
}

void fire_jumping_obstacle_init(FireRollingObstacle* obstacle, fw64Node* reference, Player* player, fw64Mesh* flame_mesh);
int fire_jumping_obstacle_update(FireRollingObstacle* obstacle, float time_delta);
void fire_jumping_obstacle_draw(FireRollingObstacle* obstacle, fw64Renderer* renderer);

#ifdef __cplusplus
}
#endif