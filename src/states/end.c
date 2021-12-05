#include "end.h"

void end_screen_init(EndScreen* end_screen, fw64Engine* engine, GameStateData* game_state) {
    end_screen->engine = engine;
    end_screen->game_state = game_state;

    fw64_camera_init(&end_screen->camera);
}

void end_screen_update(EndScreen* end_screen) {

}

void end_screen_draw(EndScreen* end_screen) {
    fw64_renderer_begin(end_screen->engine->renderer, &end_screen->camera, FW64_RENDERER_MODE_TRIANGLES, FW64_RENDERER_FLAG_CLEAR);
    fw64_renderer_end(end_screen->engine->renderer, FW64_RENDERER_FLAG_SWAP);
}

void end_screen_uninit(EndScreen* end_screen) {

}