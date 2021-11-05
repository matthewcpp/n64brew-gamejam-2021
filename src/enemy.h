#pragma once

#include "player.h"

#include "framework64/engine.h"
#include "framework64/node.h"

typedef struct {
    fw64Engine* engine;
    Player* player;
    fw64Node node;
    fw64Collider collider;
    float speed;
} Enemy;

#ifdef __cplusplus
extern "C" {
#endif

void enemy_init(Enemy* enemy, fw64Engine* engine, Player* player, fw64Mesh* mesh, Vec3* position);
void enemy_update(Enemy* enemy);
void enemy_draw(Enemy* enemy);


#ifdef __cplusplus
}
#endif