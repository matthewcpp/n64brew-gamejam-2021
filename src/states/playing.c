#include "playing.h"

static void transition_level(PlayingState* playing);

void playing_state_init(PlayingState* playing, fw64Engine* engine, GameStateData* game_state) {
    playing->engine = engine;
    playing->game_state = game_state;

    playing->current_level = LEVEL_NONE;

    playing_set_current_level(playing, game_state->transition_level);
}

void playing_set_current_level(PlayingState* playing, LevelId level) {
    switch (playing->current_level)
    {
        case LEVEL_TUNNEL:
            tunnel_level_uninit(&playing->levels.tunnel_level);
        break;
    }

    playing->current_level = level;

    switch (playing->current_level)
    {
        case LEVEL_TUNNEL:
            tunnel_level_init(&playing->levels.tunnel_level, playing->engine);
            tunnel_level_set_game_settings(&playing->levels.tunnel_level, &playing->game_state->settings);
        break;
    }
}

void playing_state_update(PlayingState* playing) {
    if (playing->game_state->transition_level != LEVEL_NONE) {
        playing_set_current_level(playing, playing->game_state->transition_level);
        playing->game_state->transition_level = LEVEL_NONE;
    }
    
    switch (playing->current_level)
    {
        case LEVEL_TUNNEL:
            tunnel_level_update(&playing->levels.tunnel_level);
        break;
    }
}

void playing_state_draw(PlayingState* playing) {
    switch (playing->current_level)
    {
        case LEVEL_TUNNEL:
            tunnel_level_draw(&playing->levels.tunnel_level);
        break;
    }
}

void playing_state_uninit(PlayingState* playing) {
    playing_set_current_level(playing, LEVEL_NONE);
}