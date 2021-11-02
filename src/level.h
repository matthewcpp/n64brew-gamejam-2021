#pragma once

#include "level_base.h"

typedef struct {
    LevelBase base;
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
