#pragma once

#include "framework64/node.h"

#define KEY_ROTATION_SPEED 180.0f

typedef struct {
    fw64Node* node;
    float rotation;
} Key;

#ifdef __cplusplus
extern "C" {
#endif

void key_init(Key* key, fw64Node* node);
void key_update(Key* key, float time_delta);

#ifdef __cplusplus
}
#endif