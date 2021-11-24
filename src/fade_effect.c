#include "fade_effect.h"

#include "framework64/util/renderer_util.h"

void fade_effect_init(FadeEffect* fade) {
    fade->current_time = 0.0f;
    fade->duration = 0.0f;
    fade->direction = FADE_NONE;
    fade->callback = NULL;
}

void fade_effect_start(FadeEffect* fade, FadeDirection direction, float duration) {
    fade->current_time = 0.0f;
    fade->duration = duration;
    fade->direction = direction;
}

int fade_effect_is_active(FadeEffect* fade) {
    return fade->direction != FADE_NONE && fade->current_time < fade->duration;
}

void fade_effect_update(FadeEffect* fade, float time_delta) {
    if (!fade_effect_is_active(fade))
        return;

    fade->current_time += time_delta;

    if (fade->current_time >= fade->duration) {
        fade->current_time = fade->duration;

        if (fade->callback) {
            fade->callback(fade->direction);
        }
    }
}

void fade_effect_draw(FadeEffect* fade, fw64Renderer* renderer) {
    if (fade->direction == FADE_NONE)
        return;

    float t = fade->current_time / fade->duration;
    if (fade->direction == FADE_OUT)
        t = 1.0f - t;

    uint8_t alpha = (uint8_t)(t * 255.0f);

    fw64_renderer_util_fullscreen_overlay(renderer, 255, 255, 255, alpha);
}