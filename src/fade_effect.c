#include "fade_effect.h"

#include "framework64/util/renderer_util.h"

#include <string.h>

void fade_effect_init(FadeEffect* fade) {
    memset(fade, 0, sizeof(FadeEffect));
}

void fade_effect_set_callback(FadeEffect* fade, FadeEffectFunc func, void* arg) {
    fade->callback = func;
    fade->callback_arg = arg;
}

void fade_effect_start(FadeEffect* fade, FadeDirection direction, float duration) {
    fade->current_time = 0.0f;
    fade->duration = duration;
    fade->direction = direction;
}

void fade_effect_stop(FadeEffect* fade) {
    fade->direction = FADE_NONE;
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
            fade->callback(fade->direction, fade->callback_arg);
        }
    }
}

#include <stdio.h>

void fade_effect_draw(FadeEffect* fade, fw64Renderer* renderer) {
    if (fade->direction == FADE_NONE)
        return;

    float t = fade->current_time / fade->duration;
    if (fade->direction == FADE_OUT)
        t = 1.0f - t;

    uint8_t alpha = (uint8_t)(t * 255.0f);

    printf("%d\n", alpha);

    fw64_renderer_util_fullscreen_overlay(renderer, fade->color.r, fade->color.g, fade->color.b, alpha);
}
