#pragma once

#include "framework64/renderer.h"

typedef enum {
    FADE_NONE,
    FADE_OUT,
    FADE_IN
} FadeDirection;

typedef void(*FadeEffectFunc)(FadeDirection direction);

typedef struct {
    float current_time;
    float duration;
    FadeDirection direction;
    FadeEffectFunc callback;
} FadeEffect;

#ifdef __cplusplus
extern "C" {
#endif

void fade_effect_init(FadeEffect* fade);
void fade_effect_start(FadeEffect* fade, FadeDirection direction, float duration);
int fade_effect_is_active(FadeEffect* fade);
void fade_effect_update(FadeEffect* fade, float time_delta);
void fade_effect_draw(FadeEffect* fade, fw64Renderer* renderer);

#ifdef __cplusplus
}
#endif