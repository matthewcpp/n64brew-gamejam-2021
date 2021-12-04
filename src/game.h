#pragma once

#include "framework64/engine.h"

#include "states/gamestates.h"
#include "states/playing.h"
#include "states/title.h"
#include "states/end.h"


typedef union {
    TitleScreen title_screen;
    PlayingState playing;
    EndScreen end_screen;
} States;


typedef struct {
    fw64Engine* engine;
    GameState current_state;
    States states;
    GameStateData state_data;
} Game;

#ifdef __cplusplus
extern "C" {
#endif

void game_init(Game* game, fw64Engine* engine);
void game_update(Game* game);
void game_draw(Game* game);

void game_set_current_state(Game* game, GameState next_state);

void game_set_current_level(Game* game, LevelId levelId);
void game_update_playing(Game* game);
void game_draw_playing(Game* game);

#ifdef __cplusplus
}
#endif
