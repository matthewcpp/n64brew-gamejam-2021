#pragma once

#include "scene_manager.h"

typedef struct {
    SceneManager scene_manager;
} Level;

#ifdef __cplusplus
extern "C" {
#endif

void level_init(Level* level, fw64Engine* engine);
void level_uninit(Level* level);
void level_update(Level* level);
void level_draw(Level* level);

#ifdef __cplusplus
}
#endif
