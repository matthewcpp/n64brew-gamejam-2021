#pragma once

#include "framework64/color.h"
#include "framework64/engine.h"
#include "framework64/mesh.h"

#define PALETTE_TEXTURE_PRIMITIVE_INDEX_COUNT 4

typedef struct {
    fw64Mesh* player_mesh;
    fw64Texture* primary_textures[PALETTE_TEXTURE_PRIMITIVE_INDEX_COUNT];
    fw64Texture* secondary_textures[PALETTE_TEXTURE_PRIMITIVE_INDEX_COUNT];
} PlayerPalette;

#ifdef __cplusplus
extern "C" {
#endif

void player_palette_init(PlayerPalette* palette, fw64Engine* engine, fw64Mesh* player_mesh, fw64Allocator* allocator);
void player_palette_uninit(PlayerPalette* palette, fw64Engine* engine, fw64Allocator* allocator);

void player_palette_activate_primary(PlayerPalette* palette);
void player_palette_activate_secondary(PlayerPalette* palette);

#ifdef __cplusplus
}
#endif