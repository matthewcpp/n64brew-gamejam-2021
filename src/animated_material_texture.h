#pragma once 

#include "framework64/material.h"

typedef struct {
    fw64Material* material;
    float current_frame_time;
    float frame_duration;
} AnimatedMaterialTexture;

#ifdef __cplusplus
extern "C" {
#endif

void animated_material_texture_init(AnimatedMaterialTexture* animated_material, fw64Material* material, float frame_duration);
void animated_material_texture_update(AnimatedMaterialTexture* animated_material, float time_delta);

#ifdef __cplusplus
}
#endif