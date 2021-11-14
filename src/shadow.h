#pragma once

#include "framework64/engine.h"
#include "framework64/node.h"
#include "framework64/util/quad.h"

typedef struct {
    int is_active;
    fw64Engine* engine;
    fw64Mesh* quad;
    fw64Node node;
} Shadow;

#ifdef __cplusplus
extern "C" {
#endif


void shadow_init(Shadow* shadow, fw64Engine* engine);
void shadow_draw(Shadow* shadow);

#ifdef __cplusplus
}
#endif