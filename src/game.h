#pragma once

#include "framework64/engine.h"
#include "initial/level.h"
#include "tunnel/tunnel_level.h"

typedef union {
    Level level;
    TunnelLevel tunnel_level;
} Levels;

typedef enum {
    LEVEL_NONE,
    LEVEL_SIMPLE_SCENE,
    LEVEL_TUNNEL
} LevelId;

typedef enum {
    GAME_STATE_PLAYING
} GameState;

typedef struct {
    fw64Engine* engine;
    GameState state;
    Levels levels;
    LevelId currentLevel;
} Game;

#ifdef __cplusplus
extern "C" {
#endif

void game_init(Game* game, fw64Engine* engine);
void game_update(Game* game);
void game_draw(Game* game);

void game_set_current_level(Game* game, LevelId levelId);
void game_update_playing(Game* game);
void game_draw_playing(Game* game);

#ifdef __cplusplus
}
#endif
