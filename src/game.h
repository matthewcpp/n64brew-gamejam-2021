#pragma once

#include "framework64/engine.h"
#include "framework64/node.h"
#include "level1.h"

typedef struct {
    fw64Engine* engine;
    Level1 level;
} Game;

#ifdef __cplusplus
extern "C" {
#endif

void game_init(Game* game, fw64Engine* engine);
void game_update(Game* game);
void game_draw(Game* game);

#ifdef __cplusplus
}
#endif
