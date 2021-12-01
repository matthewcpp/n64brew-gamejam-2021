#pragma once

#include "player.h"

typedef struct {
    fw64Engine* engine;
    fw64Allocator* allocator;
    Player* player;
    fw64Node* node;
} Girl;

#ifdef __cplusplus
extern "C" {
#endif

void girl_init(Girl* girl, fw64Node* node, Player* player, fw64Engine* engine, fw64Allocator* allocator);
void girl_uninit(Girl* girl);
void girl_update(Girl* girl);
void girl_draw(Girl* girl);

#ifdef __cplusplus
}
#endif