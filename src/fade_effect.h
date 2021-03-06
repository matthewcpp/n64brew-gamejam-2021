#pragma once

#include "framework64/renderer.h"
#include "framework64/color.h"

typedef enum {
    FADE_NONE,
    FADE_OUT,
    FADE_IN
} FadeDirection;

typedef void(*FadeEffectFunc)(FadeDirection direction, void* arg);

typedef struct {
    float current_time;
    float duration;
    fw64ColorRGBA8 color;
    FadeDirection direction;
    FadeEffectFunc callback;
    void* callback_arg;
} FadeEffect;

#ifdef __cplusplus
extern "C" {
#endif

void fade_effect_init(FadeEffect* fade);
void fade_effect_set_callback(FadeEffect* fade, FadeEffectFunc func, void* arg);
void fade_effect_start(FadeEffect* fade, FadeDirection direction, float duration);
void fade_effect_stop(FadeEffect* fade);
int fade_effect_is_active(FadeEffect* fade);
void fade_effect_update(FadeEffect* fade, float time_delta);
void fade_effect_draw(FadeEffect* fade, fw64Renderer* renderer);

#ifdef __cplusplus
}
#endif