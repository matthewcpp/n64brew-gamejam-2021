#pragma once

#include "framework64/engine.h"
#include "framework64/node.h"

typedef struct {
    fw64Engine* engine;
    fw64Node node;

    float move_speed;
    float rotation_speed;
    float rotation;
} Player;

#ifdef __cplusplus
extern "C" {
#endif

void player_init(Player* player, fw64Engine* engine);
void player_update(Player* player);
void player_draw(Player* player);

#ifdef __cplusplus
}
#endif