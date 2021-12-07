#include "game.h"

#include "assets.h"

void game_init(Game* game, fw64Engine* engine) {
    game->engine = engine;
    game->current_state = GAME_STATE_NONE;

    game_state_data_init(&game->state_data);
    game->music_bank = fw64_music_bank_load(engine->assets, FW64_ASSET_musicbank_music, NULL);
    fw64_audio_set_music_bank(engine->audio, game->music_bank);

    game->sound_bank = fw64_sound_bank_load(engine->assets, FW64_ASSET_soundbank_sound_effects, NULL);
    fw64_audio_set_sound_bank(engine->audio, game->sound_bank);

    game_set_current_state(game, GAME_STATE_TITLE);
}

void game_update(Game* game){
    if (game->state_data.transition_state != GAME_STATE_NONE) {
        game_set_current_state(game, game->state_data.transition_state);
        game->state_data.transition_state = GAME_STATE_NONE;
    }

    switch(game->current_state) {
        case GAME_STATE_TITLE:
            title_screen_update(&game->states.title_screen);
            break;

        case GAME_STATE_PLAYING:
            playing_state_update(&game->states.playing);
        break;

        case GAME_STATE_END:
            end_screen_update(&game->states.end_screen);
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

        case GAME_STATE_END:
            end_screen_draw(&game->states.end_screen);
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

        case GAME_STATE_END:
            end_screen_uninit(&game->states.end_screen);
        break;
    }

    game->current_state = next_state;

        switch (game->current_state) {
        case GAME_STATE_TITLE:
            title_screen_init(&game->states.title_screen, game->engine, &game->state_data);
        break;

        case GAME_STATE_PLAYING:
            playing_state_init(&game->states.playing, game->engine, &game->state_data);
        break;

        case GAME_STATE_END:
            end_screen_init(&game->states.end_screen, game->engine, &game->state_data);
        break;
    }
}
