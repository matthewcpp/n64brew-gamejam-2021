#include "title.h"

#include "assets.h"

#include "framework64/n64/controller_button.h"

void title_screen_init(TitleScreen* title_screen, fw64Engine* engine, GameStateData* game_state){
        title_screen->engine = engine;
        title_screen->game_state = game_state;

        ui_navigation_init(&title_screen->ui_navigation, engine->input);

        title_screen->title_font = fw64_font_load(engine->assets, FW64_ASSET_font_title_font, NULL);
        title_screen->menu_font = fw64_font_load(engine->assets, FW64_ASSET_font_title_menu_font, NULL);

        title_screen->menu_selection = 0;

        fw64_camera_init(&title_screen->camera);

        title_screen->measurements[0] = fw64_font_measure_text(title_screen->title_font, "title");
        title_screen->measurements[1] = fw64_font_measure_text(title_screen->title_font, "screen");
        title_screen->measurements[2] = fw64_font_measure_text(title_screen->menu_font, "single player");
        title_screen->measurements[3] = fw64_font_measure_text(title_screen->menu_font, "multiplayer");

        title_screen->indicator_texture = fw64_texture_create_from_image(fw64_image_load(engine->assets, FW64_ASSET_image_title_indicator, NULL), NULL);
}

void title_screen_update(TitleScreen* title_screen){
    ui_navigation_update(&title_screen->ui_navigation);

    if (title_screen->menu_selection == 0 && ui_navigation_moved_down(&title_screen->ui_navigation)) {
        title_screen->menu_selection = 1;
    }
    else if (ui_navigation_moved_up(&title_screen->ui_navigation)) {
        title_screen->menu_selection = 0;
    }

    if (ui_navigation_accepted(&title_screen->ui_navigation)) {
        title_screen->game_state->transition_state = GAME_STATE_PLAYING;
        title_screen->game_state->transition_level = LEVEL_TUNNEL;
    }
}

void title_screen_draw(TitleScreen* title_screen){
    fw64Renderer* renderer = title_screen->engine->renderer;
    IVec2 screen_size;

    fw64_renderer_get_screen_size(renderer, &screen_size);
    fw64_renderer_begin(renderer, &title_screen->camera, FW64_RENDERER_MODE_ORTHO2D, FW64_RENDERER_FLAG_CLEAR);

    int x_pos = (screen_size.x / 2) - (title_screen->measurements[0].x / 2);
    fw64_renderer_draw_text(renderer, title_screen->title_font, x_pos,5, "title");
    x_pos = (screen_size.x / 2) - (title_screen->measurements[1].x / 2);
    fw64_renderer_draw_text(renderer, title_screen->title_font, x_pos, 42, "screen");

    x_pos = (screen_size.x / 2) - (title_screen->measurements[2].x / 2);
    fw64_renderer_draw_text(renderer, title_screen->menu_font, x_pos, 140, "single player");

    if (title_screen->menu_selection == 0) {
        fw64_renderer_draw_sprite_slice(renderer, title_screen->indicator_texture, 0, x_pos - 22, 140);
        fw64_renderer_draw_sprite_slice(renderer, title_screen->indicator_texture, 1, x_pos + title_screen->measurements[2].x + 6, 140);
    }
        
    x_pos = (screen_size.x / 2) - (title_screen->measurements[3].x / 2);
    fw64_renderer_draw_text(renderer, title_screen->menu_font, x_pos, 175, "multiplayer");

    if (title_screen->menu_selection == 1) {
        fw64_renderer_draw_sprite_slice(renderer, title_screen->indicator_texture, 0, x_pos - 22, 175);
        fw64_renderer_draw_sprite_slice(renderer, title_screen->indicator_texture, 1, x_pos + title_screen->measurements[3].x + 6, 175);
    }

    fw64_renderer_end(renderer, FW64_RENDERER_FLAG_SWAP);
}

void title_screen_uninit(TitleScreen* title_screen) {
    fw64Image* indicators = fw64_texture_get_image(title_screen->indicator_texture);

    fw64_font_delete(title_screen->engine->assets, title_screen->title_font, NULL);
    fw64_font_delete(title_screen->engine->assets, title_screen->menu_font, NULL);
    fw64_image_delete(title_screen->engine->assets, indicators, NULL);
    fw64_texture_delete(title_screen->indicator_texture, NULL);
}

