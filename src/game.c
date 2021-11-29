#include "game.h"

#include "assets.h"

void game_init(Game* game, fw64Engine* engine) {
    game->engine = engine;
    game->current_state = GAME_STATE_NONE;
    game_set_current_state(game, GAME_STATE_TITLE);
}

void game_update(Game* game){
    switch(game->current_state) {
        case GAME_STATE_TITLE:
            title_screen_update(&game->states.title_screen);

        case GAME_STATE_PLAYING:
            playing_state_update(&game->states.playing);
        break;
    }
}

void game_draw(Game* game) {
    switch(game->current_state) {
        case GAME_STATE_TITLE:
            title_screen_draw(&game->states.title_screen);
        break;
        case GAME_STATE_PLAYING:
            playing_state_draw(&game->states.playing);
        break;
    }
}

void game_set_current_state(Game* game, GameState next_state) {
    switch (game->current_state) {
        case GAME_STATE_TITLE:
            title_screen_uninit(&game->states.title_screen);
        break;

        case GAME_STATE_PLAYING:
            playing_state_uninit(&game->states.playing);
        break;
    }

    game->current_state = next_state;

        switch (game->current_state) {
        case GAME_STATE_TITLE:
            title_screen_init(&game->states.title_screen, game->engine);
        break;

        case GAME_STATE_PLAYING:
            playing_state_init(&game->states.playing, game->engine);
        break;
    }
}
