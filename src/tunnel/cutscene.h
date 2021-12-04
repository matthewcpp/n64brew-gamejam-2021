#pragma once 

#include "fade_effect.h"
#include "dialogue_window.h"
#include "tunnel_level.h"

#include "framework64/engine.h"
#include "framework64/scene.h"

typedef enum {
    CUTSCENE_INACTIVE,
    CUTSCENE_FADING_OUT,
    CUTSCENE_BLANK,
    CUTSCENE_FADING_IN,
    CUTSCENE_DIALOGUE,
    CUTSCENE_DIALOGUE_COMPLETE,
    CUTSCENE_FINSISHING,
    CUTSCENE_WAITING,
    CUTSCENE_COMPLETE
} CutsceneState;

typedef struct {
    fw64Engine* engine;
    TunnelLevel* level;
    fw64Scene* scene;
    DialogueWindow dialogue;
    CutsceneState state;
    float current_state_time;
    float total_state_time;
} Cutscene;

#ifdef __cplusplus
extern "C" {
#endif

void cutscene_init(Cutscene* cutscene, fw64Engine* engine, TunnelLevel* level, fw64Scene* scene);
void cutscene_uninit(Cutscene* cutscene);
void cutscene_update(Cutscene* cutscene);
void cutscene_ui_draw(Cutscene* cutscene);

void cutscene_start(Cutscene* cutscene);
int cutscene_is_active(Cutscene* cutscene);

#ifdef __cplusplus
}
#endif