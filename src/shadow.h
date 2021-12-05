#pragma once

#include "framework64/engine.h"
#include "framework64/node.h"
#include "framework64/scene.h"
#include "framework64/util/quad.h"

typedef struct {
    int is_active;
    fw64Engine* engine;
    fw64Mesh* quad;
    fw64Node node;
    fw64Scene* scene;
    fw64Transform* target;
} Shadow;

#ifdef __cplusplus
extern "C" {
#endif


void shadow_init(Shadow* shadow, fw64Engine* engine, fw64Scene* scene, fw64Transform* target, fw64Allocator* allocator);
void shadow_uninit(Shadow* shadow, fw64Allocator* allocator);
void shadow_update(Shadow* shadow);
void shadow_draw(Shadow* shadow);

#ifdef __cplusplus
}
#endif