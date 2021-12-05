#include "ui.h"

#include "assets.h"

#include <stdio.h>

void ui_init(UI* ui, fw64Engine* engine, Player* player, fw64Allocator* allocator) {
    ui->engine = engine;
    ui->player = player;
    ui->active = 1;

    ui->font = fw64_font_load(engine->assets, FW64_ASSET_font_Consolas12, allocator);
}

void ui_uninit(UI* ui, fw64Allocator* allocator) {
    fw64_font_delete(ui->engine->assets, ui->font, allocator);
}

void ui_update(UI* ui) {
    if (!ui->active)
        return;
    Vec3* pos = &ui->player->node.transform.position;
    const char* status_text;
    const char* dashing_text;

    switch (ui->player->state) {
        case PLAYER_STATE_ON_GROUND:
            status_text = "Ground";
            break;

        case PLAYER_STATE_FALLING:
            status_text = "Falling";
            break;
    }

    switch (ui->player->is_dashing) {
        case 0:
            dashing_text = "Normal";
            break;

        case 1:
            dashing_text = "Dashing";
            break;
    }

    sprintf(ui->status_text, "%.3f %.3f %.3f %s %.3f %s", pos->x, pos->y, pos->z, status_text, ui->player->air_velocity, dashing_text);
}

void ui_draw(UI* ui) {
    if (!ui->active)
    return;
    fw64_renderer_draw_text(ui->engine->renderer, ui->font, 10,10, ui->status_text);
}