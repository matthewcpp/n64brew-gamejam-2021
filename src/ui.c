#include "ui.h"

#include "assets.h"

#include <stdio.h>

void ui_init(UI* ui, fw64Engine* engine, Player* player, ChaseCamera* camera) {
    ui->engine = engine;
    ui->player = player;
    ui->camera = camera;
    ui->mode = UI_MODE_NONE;
    ui->font = fw64_font_load(engine->assets, FW64_ASSET_font_Consolas12, NULL);
    ui->timer = 0.0f;
    ui->timer_running = 1;
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
    int cont_conn[4];

    for(int i = 0; i < 4; i++) {
        if(fw64_input_controller_is_connected(ui->engine->input, i)) {
            cont_conn[i] = 1;
            continue;
        }
        cont_conn[i] = 0;
    }
    sprintf(ui->status_text, "%.3f %d %d %d %d %d %d", ui->timer, cont_conn[0], cont_conn[1], cont_conn[2], cont_conn[3], ui->player->controller_num, ui->player->deaths);
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
    fw64_renderer_draw_text(ui->engine->renderer, ui->font, 10,10, ui->status_text);
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