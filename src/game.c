#include "game.h"
#include "assets.h"

float rotation = 0.0f;

void game_init(Game* game, fw64Engine* engine) {
    game->engine = engine;
    level1_init(&game->level, engine);
}

void game_update(Game* game){
    level1_update(&game->level);
}

void game_draw(Game* game) {
    level1_draw(&game->level);
}
