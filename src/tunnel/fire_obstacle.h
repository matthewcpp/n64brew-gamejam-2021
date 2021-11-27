#pragma once

#include "player.h"

typedef enum  {
    FIRE_OBSTACLE_DEFAULT,
    FIRE_OBSTACLE_ROLL
}FireObstacleType;


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
    FireObstacleType type;
    fw64Node* nodes[ROLL_OBSTACLE_NODE_COUNT];
    Box box;
} FireObstacle;

#ifdef __cplusplus
extern "C" {
#endif

void fire_obstacle_init(FireObstacle* obstacle, FireObstacleType type, fw64Node* reference, Player* player, FireObstacleNodePool* pool);
int fire_obstacle_update(FireObstacle* obstacle, float time_delta);
void fire_obstacle_draw(FireObstacle* obstacle, fw64Renderer* renderer);

#ifdef __cplusplus
}
#endif