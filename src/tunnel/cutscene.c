#include "cutscene.h"

#include "assets.h"
#include "states/gamestates.h"


void cutscene_init(Cutscene* cutscene, fw64Engine* engine, TunnelLevel* level, fw64Scene* scene) {
    cutscene->engine = engine;
    cutscene->level = level;
    cutscene->scene = scene;
    cutscene->state = CUTSCENE_INACTIVE;

    IVec2 pos = {20, 181};
    IVec2 size = {280, 40};
    dialogue_window_init(&cutscene->dialogue, engine, FW64_ASSET_font_Dialogue, FW64_ASSET_raw_dialogue, &pos, &size, fw64_scene_get_allocator(scene));
}

void cutscene_uninit(Cutscene* cutscene) {
    dialogue_window_uninit(&cutscene->dialogue, fw64_scene_get_allocator(cutscene->scene));
}

static void fade_in_complete(FadeDirection direction, void* arg) {
    Cutscene* cutscene = (Cutscene*)arg;
    cutscene->state = CUTSCENE_DIALOGUE;
    dialogue_window_start(&cutscene->dialogue);
}

static void cutscene_update_blank(Cutscene* cutscene) {
    cutscene->current_state_time += cutscene->engine->time->time_delta;

    if (cutscene->current_state_time >= cutscene->total_state_time) {
        cutscene->state = CUTSCENE_FADING_IN;
        fade_effect_set_callback(&cutscene->level->fade_effect, fade_in_complete, cutscene);
        fade_effect_start(&cutscene->level->fade_effect, FADE_IN, 2.0f);

        // TODO: set character positions and camera
    }
}

static void cutscene_finishing_complete(FadeDirection direction, void* data) {
    Cutscene* cutscene = (Cutscene*)data;

    cutscene->state = CUTSCENE_WAITING;
    cutscene->current_state_time = 0.0f;
    cutscene->total_state_time = 1.0f;
}

static void cutscene_update_dialogue(Cutscene* cutscene) {
    dialogue_window_update(&cutscene->dialogue);

    if (cutscene->dialogue.status == DIALOGUE_WINDOW_STATUS_DONE) {
        cutscene->state = CUTSCENE_DIALOGUE_COMPLETE;
        fade_effect_set_callback(&cutscene->level->fade_effect, cutscene_finishing_complete, cutscene);
        fade_effect_start(&cutscene->level->fade_effect, FADE_OUT, 2.0f);
    }
}

void cutscene_update_waiting(Cutscene* cutscene) {
    cutscene->current_state_time += cutscene->engine->time->time_delta;

    if (cutscene->current_state_time >= cutscene->total_state_time) {
        cutscene->state = CUTSCENE_COMPLETE;
    }
}

void cutscene_update(Cutscene* cutscene) {
    switch (cutscene->state)
    {
    case CUTSCENE_BLANK:
        cutscene_update_blank(cutscene);
        break;

    case CUTSCENE_DIALOGUE:
        cutscene_update_dialogue(cutscene);
    break;

    case CUTSCENE_WAITING:
        cutscene_update_waiting(cutscene);
    break;
    
    default:
        break;
    }
}

void cutscene_ui_draw(Cutscene* cutscene) {
    if (cutscene->state == CUTSCENE_DIALOGUE) {
        dialogue_window_draw(&cutscene->dialogue);
    }
}

static void initial_fade_out_complete(FadeDirection direction, void* arg) {
    Cutscene* cutscene = (Cutscene*)arg;
    cutscene->state = CUTSCENE_BLANK;
    cutscene->current_state_time = 0.0f;
    cutscene->total_state_time = 1.0f;
}

void cutscene_start(Cutscene* cutscene) {
    cutscene->state = CUTSCENE_FADING_OUT;

    fw64ColorRGBA8 black = {0, 0, 0, 255};
    cutscene->level->fade_effect.color = black;
    fade_effect_set_callback(&cutscene->level->fade_effect, initial_fade_out_complete, cutscene);
    fade_effect_start(&cutscene->level->fade_effect, FADE_OUT, 2.0f);
}

int cutscene_is_active(Cutscene* cutscene) {
    return cutscene->state != CUTSCENE_INACTIVE;
}