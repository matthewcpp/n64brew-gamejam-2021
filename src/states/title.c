#include "title.h"

#include "assets.h"

#include "framework64/n64/controller_button.h"

void title_screen_init(TitleScreen* title_screen, fw64Engine* engine, GameStateData* game_state){
        title_screen->engine = engine;
        title_screen->game_state = game_state;

        ui_navigation_init(&title_screen->ui_navigation, engine->input);

        title_screen->title_font = fw64_font_load(engine->assets, FW64_ASSET_font_title_font, NULL);
        title_screen->menu_font = fw64_font_load(engine->assets, FW64_ASSET_font_title_menu_font, NULL);

        title_screen->menu_selection = MENU_CHOICE_SINGLE_PLAYER;

        fw64_camera_init(&title_screen->camera);

        title_screen->measurements[0] = fw64_font_measure_text(title_screen->title_font, "title");
        title_screen->measurements[1] = fw64_font_measure_text(title_screen->title_font, "screen");
        title_screen->measurements[2] = fw64_font_measure_text(title_screen->menu_font, "single player");
        title_screen->measurements[3] = fw64_font_measure_text(title_screen->menu_font, "multiplayer");
        title_screen->measurements[4] = fw64_font_measure_text(title_screen->menu_font, "controlled");
        title_screen->measurements[5] = fw64_font_measure_text(title_screen->menu_font, "fixed");
        title_screen->measurements[6] = fw64_font_measure_text(title_screen->menu_font, "fast");
        title_screen->measurements[7] = fw64_font_measure_text(title_screen->menu_font, "med");
        title_screen->measurements[8] = fw64_font_measure_text(title_screen->menu_font, "slow");

        title_screen->indicator_texture = fw64_texture_create_from_image(fw64_image_load(engine->assets, FW64_ASSET_image_title_indicator, NULL), NULL);
}

void title_screen_update(TitleScreen* title_screen){
    ui_navigation_update(&title_screen->ui_navigation);

    if (title_screen->menu_selection == MENU_CHOICE_SINGLE_PLAYER && ui_navigation_moved_down(&title_screen->ui_navigation)) {
        title_screen->menu_selection = MENU_CHOICE_MULTIPLAYER;
    }
    else if (title_screen->menu_selection == MENU_CHOICE_MULTIPLAYER && ui_navigation_moved_up(&title_screen->ui_navigation)) {
        title_screen->menu_selection = MENU_CHOICE_SINGLE_PLAYER;
    }

    if(ui_navigation_moved_left(&title_screen->ui_navigation)) {
        switch(title_screen->menu_selection) {
            case(MENU_CHOICE_MULTI_CONTROLLED):
                title_screen->menu_selection = MENU_CHOICE_MULTI_FIXED;
                break;
            case(MENU_CHOICE_MULTI_FIXED):
                title_screen->menu_selection = MENU_CHOICE_MULTI_CONTROLLED;
                break;
            case(MENU_CHOICE_MULTI_FIXED_FAST):
                title_screen->menu_selection = MENU_CHOICE_MULTI_FIXED_MED;
                break;
            case(MENU_CHOICE_MULTI_FIXED_MED):
                title_screen->menu_selection = MENU_CHOICE_MULTI_FIXED_SLOW;
                break;
            case(MENU_CHOICE_MULTI_FIXED_SLOW):
                title_screen->menu_selection = MENU_CHOICE_MULTI_FIXED_FAST;
                break;
            default:
                break;
        }
    }
    else if(ui_navigation_moved_right(&title_screen->ui_navigation)) {
        switch(title_screen->menu_selection) {
            case(MENU_CHOICE_MULTI_CONTROLLED):
                title_screen->menu_selection = MENU_CHOICE_MULTI_FIXED;
                break;
            case(MENU_CHOICE_MULTI_FIXED):
                title_screen->menu_selection = MENU_CHOICE_MULTI_CONTROLLED;
                break;
            case(MENU_CHOICE_MULTI_FIXED_FAST):
                title_screen->menu_selection = MENU_CHOICE_MULTI_FIXED_SLOW;
                break;
            case(MENU_CHOICE_MULTI_FIXED_MED):
                title_screen->menu_selection = MENU_CHOICE_MULTI_FIXED_FAST;
                break;
            case(MENU_CHOICE_MULTI_FIXED_SLOW):
                title_screen->menu_selection = MENU_CHOICE_MULTI_FIXED_MED;
                break;
            default:
                break;
        }
    }

    if (ui_navigation_accepted(&title_screen->ui_navigation)) {
        switch(title_screen->menu_selection)
        {
            case(MENU_CHOICE_SINGLE_PLAYER): { //select single player
                title_screen->game_state->settings.control_mode = CONTROL_MODE_SINGLE;
                title_screen->game_state->transition_state = GAME_STATE_PLAYING;
                title_screen->game_state->transition_level = LEVEL_TUNNEL;
                break;
            }
            case(MENU_CHOICE_MULTIPLAYER): { //select multiplayer, show controlled/fixed choice
                title_screen->menu_selection = MENU_CHOICE_MULTI_FIXED;
                break;
            }
            case(MENU_CHOICE_MULTI_CONTROLLED): { //select multiplayer controlled
                title_screen->game_state->settings.control_mode = CONTROL_MODE_MULTI_ONPRESS;
                title_screen->game_state->transition_state = GAME_STATE_PLAYING;
                title_screen->game_state->transition_level = LEVEL_TUNNEL;
                break;
            }
            case(MENU_CHOICE_MULTI_FIXED): { //select multiplayer fixed, show fast/med/slow
                title_screen->menu_selection = MENU_CHOICE_MULTI_FIXED_MED;
                break;
            }
            case(MENU_CHOICE_MULTI_FIXED_FAST): { //select multiplayer fixed, fast
                title_screen->game_state->settings.control_mode = CONTROL_MODE_MULTI_TIMER;
                title_screen->game_state->settings.control_transfer_time = CONTROL_TRANSFER_TIME_FAST;
                title_screen->game_state->transition_state = GAME_STATE_PLAYING;
                title_screen->game_state->transition_level = LEVEL_TUNNEL;
                break;
            }
            case(MENU_CHOICE_MULTI_FIXED_MED): { //select multiplayer fixed, med
                title_screen->game_state->settings.control_mode = CONTROL_MODE_MULTI_TIMER;
                title_screen->game_state->settings.control_transfer_time = CONTROL_TRANSFER_TIME_MED;
                title_screen->game_state->transition_state = GAME_STATE_PLAYING;
                title_screen->game_state->transition_level = LEVEL_TUNNEL;
                break;
            }
            case(MENU_CHOICE_MULTI_FIXED_SLOW): { //select multiplayer fixed, slow
                title_screen->game_state->settings.control_mode = CONTROL_MODE_MULTI_TIMER;
                title_screen->game_state->settings.control_transfer_time = CONTROL_TRANSFER_TIME_SLOW;
                title_screen->game_state->transition_state = GAME_STATE_PLAYING;
                title_screen->game_state->transition_level = LEVEL_TUNNEL;
                break;
            }
            default:
                break;
        }
    }

    if (ui_navigation_back(&title_screen->ui_navigation)) {
        switch(title_screen->menu_selection)
        {
            case(MENU_CHOICE_MULTI_CONTROLLED): /* fall through */
            case(MENU_CHOICE_MULTI_FIXED): { //return to "multiplayer"
                title_screen->menu_selection = MENU_CHOICE_MULTIPLAYER;
                break;
            }
            case(MENU_CHOICE_MULTI_FIXED_FAST): /* fall through */ 
            case(MENU_CHOICE_MULTI_FIXED_MED):  /* fall through */ 
            case(MENU_CHOICE_MULTI_FIXED_SLOW): { //return to "multiplayer fixed"
                title_screen->menu_selection = MENU_CHOICE_MULTI_FIXED;
                break;
            }
            default:
                break;
        }
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
    fw64_renderer_draw_text(renderer, title_screen->menu_font, x_pos, 105, "single player");

    x_pos = (screen_size.x / 2) - (title_screen->measurements[3].x / 2);
    fw64_renderer_draw_text(renderer, title_screen->menu_font, x_pos, 140, "multiplayer");

    switch(title_screen->menu_selection)
    {
        case(MENU_CHOICE_SINGLE_PLAYER): {
            x_pos = (screen_size.x / 2) - (title_screen->measurements[2].x / 2);
            fw64_renderer_draw_sprite_slice(renderer, title_screen->indicator_texture, 0, x_pos - 22, 105);
            fw64_renderer_draw_sprite_slice(renderer, title_screen->indicator_texture, 1, x_pos + title_screen->measurements[2].x + 6, 105);
            break;
        }
        case(MENU_CHOICE_MULTIPLAYER): {
            x_pos = (screen_size.x / 2) - (title_screen->measurements[3].x / 2);
            fw64_renderer_draw_sprite_slice(renderer, title_screen->indicator_texture, 0, x_pos - 22, 140);
            fw64_renderer_draw_sprite_slice(renderer, title_screen->indicator_texture, 1, x_pos + title_screen->measurements[3].x + 6, 140);
            break;
        }
        case(MENU_CHOICE_MULTI_CONTROLLED): {
            x_pos = (screen_size.x / 2) - (title_screen->measurements[4].x / 2);
            fw64_renderer_draw_text(renderer, title_screen->menu_font, x_pos, 175, "controlled");
            fw64_renderer_draw_sprite_slice(renderer, title_screen->indicator_texture, 0, x_pos - 22, 175);
            fw64_renderer_draw_sprite_slice(renderer, title_screen->indicator_texture, 1, x_pos + title_screen->measurements[4].x + 6, 175);
            break;
        }
        case(MENU_CHOICE_MULTI_FIXED): {
            x_pos = (screen_size.x / 2) - (title_screen->measurements[5].x / 2);
            fw64_renderer_draw_text(renderer, title_screen->menu_font, x_pos, 175, "fixed");
            fw64_renderer_draw_sprite_slice(renderer, title_screen->indicator_texture, 0, x_pos - 22, 175);
            fw64_renderer_draw_sprite_slice(renderer, title_screen->indicator_texture, 1, x_pos + title_screen->measurements[5].x + 6, 175);
            break;
        }
        case(MENU_CHOICE_MULTI_FIXED_FAST): {
            x_pos = (screen_size.x / 2) - (title_screen->measurements[5].x / 2);
            fw64_renderer_draw_text(renderer, title_screen->menu_font, x_pos, 175, "fixed");
            x_pos = (screen_size.x / 2) - (title_screen->measurements[6].x / 2);
            fw64_renderer_draw_text(renderer, title_screen->menu_font, x_pos, 210, "fast");
            fw64_renderer_draw_sprite_slice(renderer, title_screen->indicator_texture, 0, x_pos - 22, 210);
            fw64_renderer_draw_sprite_slice(renderer, title_screen->indicator_texture, 1, x_pos + title_screen->measurements[6].x + 6, 210);
            break;
        }
        case(MENU_CHOICE_MULTI_FIXED_MED): {
            x_pos = (screen_size.x / 2) - (title_screen->measurements[5].x / 2);
            fw64_renderer_draw_text(renderer, title_screen->menu_font, x_pos, 175, "fixed");
            x_pos = (screen_size.x / 2) - (title_screen->measurements[7].x / 2);
            fw64_renderer_draw_text(renderer, title_screen->menu_font, x_pos, 210, "med");
            fw64_renderer_draw_sprite_slice(renderer, title_screen->indicator_texture, 0, x_pos - 22, 210);
            fw64_renderer_draw_sprite_slice(renderer, title_screen->indicator_texture, 1, x_pos + title_screen->measurements[7].x + 6, 210);
            break;
        }
        case(MENU_CHOICE_MULTI_FIXED_SLOW): {
            x_pos = (screen_size.x / 2) - (title_screen->measurements[5].x / 2);
            fw64_renderer_draw_text(renderer, title_screen->menu_font, x_pos, 175, "fixed");
            x_pos = (screen_size.x / 2) - (title_screen->measurements[8].x / 2);
            fw64_renderer_draw_text(renderer, title_screen->menu_font, x_pos, 210, "slow");
            fw64_renderer_draw_sprite_slice(renderer, title_screen->indicator_texture, 0, x_pos - 22, 210);
            fw64_renderer_draw_sprite_slice(renderer, title_screen->indicator_texture, 1, x_pos + title_screen->measurements[8].x + 6, 210);
            break;
        }
        default:
            break;
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

