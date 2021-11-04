#include "game.h"

#include "assets.h"

void game_init(Game* game, fw64Engine* engine) {
    game->engine = engine;
    game->state = GAME_STATE_PLAYING;
    game->currentLevel = LEVEL_NONE;

    game_set_current_level(game, LEVEL_SIMPLE_SCENE);
}

void game_update(Game* game){
    switch(game->state) {
        case GAME_STATE_PLAYING:
            game_update_playing(game);
        break;
    }
}

void game_draw(Game* game) {
    switch(game->state) {
        case GAME_STATE_PLAYING:
            game_draw_playing(game);
        break;
    }
}

void game_set_current_level(Game* game, LevelId levelId) {
    switch(game->currentLevel) {
        case LEVEL_SIMPLE_SCENE:
            level_uninit(&game->levels.level);
        break;

        case LEVEL_TUNNEL:
            tunnel_level_uninit(&game->levels.tunnel_level);
        break;
    }

    game->currentLevel = levelId;

    switch(game->currentLevel) {
        case LEVEL_SIMPLE_SCENE:
            level_init(&game->levels.level, game->engine);
        break;

        case LEVEL_TUNNEL:
            tunnel_level_init(&game->levels.tunnel_level, game->engine);
        break;
    }
}

void game_update_playing(Game* game) {
    switch(game->currentLevel) {
        case LEVEL_SIMPLE_SCENE:
            level_update(&game->levels.level);
        break;

        case LEVEL_TUNNEL:
            tunnel_level_update(&game->levels.tunnel_level);
        break;
    }
}

void game_draw_playing(Game* game) {
    switch(game->currentLevel) {
        case LEVEL_SIMPLE_SCENE:
            level_draw(&game->levels.level);
        break;

        case LEVEL_TUNNEL:
            tunnel_level_draw(&game->levels.tunnel_level);
        break;
    }
}