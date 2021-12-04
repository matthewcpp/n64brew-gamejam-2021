#include "cutscene.h"

#include "assets.h"
#include "states/gamestates.h"

#include "scene_atrium.h"


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

static void set_cutscene_positions(Cutscene* cutscene) {
        SceneRef* current_scene_ref = scene_manager_get_current(&cutscene->level->scene_manager);

        fw64Node* girl = fw64_scene_get_node(current_scene_ref->scene, FW64_scene_atrium_node_Girl);
        quat_from_euler(&girl->transform.rotation, 0.0f, 195.0f, 0.0);
        fw64_node_update(girl);


        Vec3 player_pos = {1.0f, 22.0f, -87.0f};
        vec3_add(&player_pos, &player_pos, &current_scene_ref->offset);
        cutscene->level->player.node.transform.position = player_pos;
        quat_ident(&cutscene->level->player.node.transform.rotation);
        fw64_node_update(&cutscene->level->player.node);

        Vec3 camera_pos = {0.0f, 25.0f, -85.0f};
        vec3_add(&camera_pos, &camera_pos, &current_scene_ref->offset);

        Vec3 target = {0, 25.0, -86.0};
        vec3_add(&target, &target, &current_scene_ref->offset);

        Vec3 up = {0.0f, 1.0f, 0.0f};

        cutscene->level->chase_cam.mode = CHASE_CAMERA_MANUAL;
        cutscene->level->chase_cam.camera.transform.position = camera_pos;
        fw64_transform_look_at(&cutscene->level->chase_cam.camera.transform, &target, &up);
        fw64_camera_update_view_matrix(&cutscene->level->chase_cam.camera);
}

static void cutscene_update_blank(Cutscene* cutscene) {
    cutscene->current_state_time += cutscene->engine->time->time_delta;

    if (cutscene->current_state_time >= cutscene->total_state_time) {
        set_cutscene_positions(cutscene);

        cutscene->state = CUTSCENE_FADING_IN;
        fade_effect_set_callback(&cutscene->level->fade_effect, fade_in_complete, cutscene);
        fade_effect_start(&cutscene->level->fade_effect, FADE_IN, 2.0f);
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

    cutscene->level->player.process_input = 0;
}

void cutscene_start(Cutscene* cutscene) {
    cutscene->state = CUTSCENE_FADING_OUT;

    fw64ColorRGBA8 black = {0, 0, 0, 255};
    cutscene->level->fade_effect.color = black;
    fade_effect_set_callback(&cutscene->level->fade_effect, initial_fade_out_complete, cutscene);
    fade_effect_start(&cutscene->level->fade_effect, FADE_OUT, 1.0f);
}

int cutscene_is_active(Cutscene* cutscene) {
    return cutscene->state != CUTSCENE_INACTIVE;
}