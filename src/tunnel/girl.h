#pragma once

#include "player.h"

typedef struct {
    fw64Engine* engine;
    fw64Scene* scene;
    fw64Node* node;
} Girl;

#ifdef __cplusplus
extern "C" {
#endif

void girl_init(Girl* girl, fw64Engine* engine, fw64Scene* scene, int node_index);
void girl_uninit(Girl* girl);
void girl_update(Girl* girl);
void girl_draw(Girl* girl);

#ifdef __cplusplus
}
#endif