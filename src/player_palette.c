#include "player_palette.h"

#include "assets.h"

/*
Textures
0: Face
19: Pants
24: chest
25: back
*/


static int hair_primitive_indices[] = {1, 2, 3};
static int skin_primitive_indices[] = {4, 5, 6, 7, 8};
static int texture_indices[] = {0, 19, 24, 25};
static fw64ColorRGBA8 primary_hair_color = {109, 6, 6, 255};
static fw64ColorRGBA8 primary_skin_color = {242, 164, 122, 255};
static fw64ColorRGBA8 secondary_hair_color = {52, 209, 237, 255};
static fw64ColorRGBA8 secondary_skin_color = {133, 97, 69, 255};

void player_palette_init(PlayerPalette* palette, fw64Engine* engine, fw64Mesh* player_mesh, fw64Allocator* allocator) {
    palette->player_mesh = player_mesh;

    for (int i = 0; i < PALETTE_TEXTURE_PRIMITIVE_INDEX_COUNT; i++) {
        fw64Material* material = fw64_mesh_get_material_for_primitive(player_mesh, texture_indices[i]);
        palette->primary_textures[i] = fw64_material_get_texture(material); 
    }

    fw64Material* material = fw64_mesh_get_material_for_primitive(player_mesh, skin_primitive_indices[0]);
    fw64_material_get_color(material, &primary_skin_color);
    material = fw64_mesh_get_material_for_primitive(player_mesh, hair_primitive_indices[0]);
    fw64_material_get_color(material, &primary_hair_color);

    palette->secondary_textures[0] = fw64_texture_create_from_image(fw64_image_load(engine->assets, FW64_ASSET_image_AltFaceTex, allocator), allocator);
    palette->secondary_textures[1] = fw64_texture_create_from_image(fw64_image_load(engine->assets, FW64_ASSET_image_AltPantsTex, allocator), allocator);
    palette->secondary_textures[2] = fw64_texture_create_from_image(fw64_image_load(engine->assets, FW64_ASSET_image_AltChestTex, allocator), allocator);
    palette->secondary_textures[3] = fw64_texture_create_from_image(fw64_image_load(engine->assets, FW64_ASSET_image_AltBackTex, allocator), allocator);
}

void player_palette_uninit(PlayerPalette* palette, fw64Engine* engine, fw64Allocator* allocator) {
    for (int i = 0; i < PALETTE_TEXTURE_PRIMITIVE_INDEX_COUNT; i++) {
        fw64Image* image = fw64_texture_get_image(palette->secondary_textures[i]);
        fw64_image_delete(engine->assets, image, allocator);
        fw64_texture_delete(palette->secondary_textures[i], allocator);
    }
}

static void set_palette(PlayerPalette* palette, fw64ColorRGBA8* hair_color, fw64ColorRGBA8* skin_color, fw64Texture* textures[]) {
    for (int i = 0; i < HAIR_PRIMITVE_INDEX_COUNT; i++) {
        fw64Material* material = fw64_mesh_get_material_for_primitive(palette->player_mesh, hair_primitive_indices[i]);
        fw64_material_set_color(material, hair_color->r, hair_color->g, hair_color->b);
    }

    for (int i = 0; i < SKIN_PRIMITIVE_INDEX_COUNT; i++) {
        fw64Material* material = fw64_mesh_get_material_for_primitive(palette->player_mesh, skin_primitive_indices[i]);
        fw64_material_set_color(material, skin_color->r, skin_color->g, skin_color->b);
    }

    for (int i = 0; i < PALETTE_TEXTURE_PRIMITIVE_INDEX_COUNT; i++) {
        fw64Material* material = fw64_mesh_get_material_for_primitive(palette->player_mesh, texture_indices[i]);
        fw64_material_set_texture(material, textures[i], 0); 
    }
}

void player_palette_activate_primary(PlayerPalette* palette) {
    set_palette(palette, &primary_hair_color, &primary_skin_color, &palette->primary_textures[0]);
}

void player_palette_activate_secondary(PlayerPalette* palette) {
    set_palette(palette, &secondary_hair_color, &secondary_skin_color, &palette->secondary_textures[0]);
}