#pragma once

#include "framework64/font.h"

#include "player.h"
#include "chase_camera.h"

typedef enum {
    UI_MODE_NONE,
    UI_MODE_NORMAL_HUD,
    UI_MODE_DEBUG_PLAYER,
    UI_MODE_DEBUG_CAMERA
} UIMode;

typedef struct {
    fw64Engine* engine;
    fw64Font* font;
    Player* player;
    ChaseCamera* camera;
    char status_text[80];
    char camera_text[80];
    UIMode mode;
    float timer;
    int timer_running;
    unsigned char controller_state[4];
    fw64Texture* ui_cont_active_texture;
    fw64Texture* ui_cont_inactive_texture;
    fw64Texture* ui_cont_unplugged_texture;
} UI;

#ifdef __cplusplus
extern "C" {
#endif

void ui_init(UI* ui, fw64Engine* engine, Player* player, ChaseCamera* camera);
void ui_uninit(UI* ui);
void ui_update(UI* ui);
void ui_draw(UI* ui);
void ui_set_mode(UI* ui, UIMode mode);
void ui_start_timer(UI* ui);
void ui_stop_timer(UI* ui);
void ui_reset_timer(UI* ui);
#ifdef __cplusplus
}
#endif