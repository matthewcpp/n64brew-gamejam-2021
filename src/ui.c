#include "ui.h"

#include "assets.h"

#include <stdio.h>

void ui_init(UI* ui, fw64Engine* engine, Player* player, ChaseCamera* camera, fw64Allocator* allocator) {
    ui->engine = engine;
    ui->player = player;
    ui->camera = camera;
    ui->mode = UI_MODE_NONE;
    ui->font = fw64_font_load(engine->assets, FW64_ASSET_font_Consolas12, NULL);
    ui->timer = 0.0f;
    ui->timer_running = 1;
    ui->ui_cont_active_texture = fw64_texture_create_from_image(fw64_image_load(engine->assets, FW64_ASSET_image_ui_active, allocator), allocator);
    ui->ui_cont_inactive_texture = fw64_texture_create_from_image(fw64_image_load(engine->assets, FW64_ASSET_image_ui_inactive, allocator), allocator);
    ui->ui_cont_unplugged_texture = fw64_texture_create_from_image(fw64_image_load(engine->assets, FW64_ASSET_image_ui_unplugged, allocator), allocator);
    ui->ui_clock_texture = fw64_texture_create_from_image(fw64_image_load(engine->assets, FW64_ASSET_image_ui_clock, allocator), allocator);
    ui->ui_skull_texture = fw64_texture_create_from_image(fw64_image_load(engine->assets, FW64_ASSET_image_ui_skull, allocator), allocator);
}

void ui_uninit(UI* ui, fw64Allocator* allocator) {
    fw64Image* ui_image = fw64_texture_get_image(ui->ui_cont_active_texture);
    fw64_image_delete(ui->engine->assets, ui_image, allocator);
    fw64_texture_delete(ui->ui_cont_active_texture, allocator);
    
    ui_image = fw64_texture_get_image(ui->ui_cont_inactive_texture);
    fw64_image_delete(ui->engine->assets, ui_image, allocator);
    fw64_texture_delete(ui->ui_cont_inactive_texture, allocator);
    
    ui_image = fw64_texture_get_image(ui->ui_cont_unplugged_texture);
    fw64_image_delete(ui->engine->assets, ui_image, allocator);
    fw64_texture_delete(ui->ui_cont_unplugged_texture, allocator);

    ui_image = fw64_texture_get_image(ui->ui_clock_texture);
    fw64_image_delete(ui->engine->assets, ui_image, allocator);
    fw64_texture_delete(ui->ui_clock_texture, allocator);

    ui_image = fw64_texture_get_image(ui->ui_skull_texture);
    fw64_image_delete(ui->engine->assets, ui_image, allocator);
    fw64_texture_delete(ui->ui_skull_texture, allocator);
    
    fw64_font_delete(ui->engine->assets, ui->font, allocator);
}

static void normal_hud_update(UI* ui);
static void debug_player_update(UI* ui);
static void debug_camera_update(UI* ui);

void ui_update(UI* ui) {
    if(ui->timer_running)
        ui->timer += ui->engine->time->time_delta;

    switch(ui->mode) {
        case(UI_MODE_NORMAL_HUD):
            normal_hud_update(ui);
            return;
        case(UI_MODE_DEBUG_PLAYER):
            debug_player_update(ui);
            return;
        case(UI_MODE_DEBUG_CAMERA):
            debug_camera_update(ui);
            return;
        case(UI_MODE_NONE): /* fall through*/
        default:
        return;
    }    
}

static void normal_hud_draw(UI* ui);
static void debug_player_draw(UI* ui);
static void debug_camera_draw(UI* ui);

void ui_draw(UI* ui) {

    switch(ui->mode) {
        case(UI_MODE_NORMAL_HUD):
            normal_hud_draw(ui);
            return;
        case(UI_MODE_DEBUG_PLAYER):
            debug_player_draw(ui);
            return;
        case(UI_MODE_DEBUG_CAMERA):
            debug_camera_draw(ui);
            return;
        case(UI_MODE_NONE): /* fall through*/
        default:
        return;
    }
}

void ui_set_mode(UI* ui, UIMode mode) {
    ui->mode = mode;
    return;
}

void ui_start_timer(UI* ui) {
    ui->timer_running = 1;
}
void ui_stop_timer(UI* ui) {
    ui->timer_running = 0;
}
void ui_reset_timer(UI* ui) {
    ui->timer = 0.0f;
}

void normal_hud_update(UI* ui) {
    for(int i = 0; i < 4; i++) {
        if(fw64_input_controller_is_connected(ui->engine->input, i)) {
            ui->controller_state[i] = 1;
            continue;
        }
        ui->controller_state[i] = 0;
    }
    int sec, min, hour;
    sec = floor(ui->timer);
    hour = sec / 3600;
    min = (sec / 60) % 60;
    sec = sec % 60;
    sprintf(ui->status_text, "%01d:%02d:%02d", hour, min, sec);
    sprintf(ui->camera_text, "%3d", ui->player->deaths);
    return;
}

void debug_player_update(UI* ui) {
    Vec3* pos = &ui->player->node.transform.position;
    const char* status_text;

    switch (ui->player->state) {
        case PLAYER_STATE_ON_GROUND:
            status_text = "Ground";
            break;

        case PLAYER_STATE_FALLING:
            status_text = "Falling";
            break;
    }

    sprintf(ui->status_text, "%.3f %.3f %.3f %s %.3f %d", pos->x, pos->y, pos->z, status_text, ui->player->air_velocity, ui->player->deaths);
    return;
}

void debug_camera_update(UI* ui) {

    Vec3* pos = &ui->camera->camera.transform.position;
    const char* status_text;
    const char* last_mode_text;
    const char* mode_text;

    status_text = "Mode:";

    switch(ui->camera->mode) {
        case CAMERA_MODE_CHASE:
            mode_text = "Chase";
            break;

        case CAMERA_MODE_SIDE:
            mode_text = "Side";
            break;

        case CAMERA_MODE_TOP:
            mode_text = "Top";
            break;

        case CAMERA_MODE_MANUAL: /* fall through */
        default:
            mode_text = "Manual";
            break;
    }
    switch(ui->camera->last_mode) {
        case CAMERA_MODE_CHASE:
            last_mode_text = "Chase";
            break;

        case CAMERA_MODE_SIDE:
            last_mode_text = "Side";
            break;

        case CAMERA_MODE_TOP:
            last_mode_text = "Top";
            break;

        case CAMERA_MODE_MANUAL: /* fall through */
        default:
            last_mode_text = "Manual";
            break;
    }
    if(ui->camera->mode_transition) {
        sprintf(ui->status_text, "%.3f %.3f %.3f %s %s to %s", pos->x, pos->y, pos->z, status_text, last_mode_text, mode_text);
    }
    else {
        sprintf(ui->status_text, "%.3f %.3f %.3f %s %s", pos->x, pos->y, pos->z, status_text, mode_text);
    }

    sprintf(ui->camera_text, "Dist: %.3f  Height: %.3f", ui->camera->target_follow_dist, ui->camera->target_follow_height);

    return;
}

void normal_hud_draw(UI* ui) {

    fw64_renderer_draw_text(ui->engine->renderer, ui->font, 36,24, ui->status_text);
    fw64_renderer_draw_sprite_slice(ui->engine->renderer, ui->ui_clock_texture, 0, 16, 16);

    fw64_renderer_draw_text(ui->engine->renderer, ui->font, 260,24, ui->camera_text);
    fw64_renderer_draw_sprite(ui->engine->renderer, ui->ui_skull_texture, 284, 20);
    

    int startx = 90;
    int offsetx = 34;
    for(int i = 0; i < 4; i++)
    {
        if(ui->controller_state[i]) {
            fw64_renderer_draw_sprite_slice(ui->engine->renderer, ui->ui_cont_inactive_texture, 0, startx + (offsetx * i), 16);
        }
        else {
            fw64_renderer_draw_sprite_slice(ui->engine->renderer, ui->ui_cont_unplugged_texture, 0, startx + (offsetx * i), 16);
        }
    }
    fw64_renderer_draw_sprite_slice(ui->engine->renderer, ui->ui_cont_active_texture, 0, startx + (offsetx * ui->player->controller_num), 16);
    return;
}

void debug_player_draw(UI* ui) {
    fw64_renderer_draw_text(ui->engine->renderer, ui->font, 10,10, ui->status_text);
    return;
}

void debug_camera_draw(UI* ui) {
    fw64_renderer_draw_text(ui->engine->renderer, ui->font, 10,10, ui->status_text);
    fw64_renderer_draw_text(ui->engine->renderer, ui->font, 10,20, ui->camera_text);
    return;
}