#include "end.h"

#define END_SCREEN_ALLOCATOR_SIZE 64 * 1024

void end_screen_init(EndScreen* end_screen, fw64Engine* engine, GameStateData* game_state) {
    end_screen->engine = engine;
    end_screen->game_state = game_state;

    fw64_camera_init(&end_screen->camera);
    ui_navigation_init(&end_screen->ui_navigation, engine->input);
    fw64_bump_allocator_init(&end_screen->allocator, END_SCREEN_ALLOCATOR_SIZE);
}

void end_screen_update(EndScreen* end_screen) {
    ui_navigation_update(&end_screen->ui_navigation);

    if (ui_navigation_accepted(&end_screen->ui_navigation)) {
        end_screen->game_state->transition_state = GAME_STATE_TITLE;
    }
}

void end_screen_draw(EndScreen* end_screen) {
    fw64_renderer_begin(end_screen->engine->renderer, &end_screen->camera, FW64_RENDERER_MODE_TRIANGLES, FW64_RENDERER_FLAG_CLEAR);
    fw64_renderer_end(end_screen->engine->renderer, FW64_RENDERER_FLAG_SWAP);
}

void end_screen_uninit(EndScreen* end_screen) {
    fw64_bump_allocator_uninit(&end_screen->allocator);
}