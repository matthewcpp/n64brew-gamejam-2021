#pragma once

#include "framework64/engine.h"
#include "framework64/node.h"
#include "framework64/util/quad.h"

#define SPARKLE_DURATION 0.25f
#define SPARKLE_FRAME_COUNT 16
#define SPARKLE_FRAME_DURATION  (SPARKLE_DURATION / (float)SPARKLE_FRAME_COUNT)

typedef struct {
    int is_active;
    float prev_time;
    float current_time;
    float frame_time;
    int frame_index;
    fw64Engine* engine;
    fw64Image* image;
    fw64Mesh* quad;
    fw64Node node;
} Sparkle;

#ifdef __cplusplus
extern "C" {
#endif


void sparkle_init(Sparkle* sparkle, fw64Engine* engine, fw64Allocator* allocator);
void sparkle_uninit(Sparkle* sparkle, fw64Allocator* allocator);
void sparkle_update(Sparkle* sparkle);
void sparkle_draw(Sparkle* sparkle);
void sparkle_start(Sparkle* sparkle);

#ifdef __cplusplus
}
#endif