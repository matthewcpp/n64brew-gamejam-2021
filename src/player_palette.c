#include "player_palette.h"

/*
Textures
0: Face
19: Pants
24: chest
25: back
*/

#define HAIR_PRIMITVE_INDEX_COUNT 3
#define SKIN_PRIMITIVE_INDEX_COUNT 5
static int hair_primitive_indices[] = {1, 2, 3};
static int skin_primitive_indices[] = {4, 5, 6, 7, 8};
static int texture_indices[] = {0, 19, 24, 25};
static fw64ColorRGBA8 primary_hair_color = {255,0,0, 255};
static fw64ColorRGBA8 primary_skin_color = {255,0,0, 255};
static fw64ColorRGBA8 secondary_hair_color = {52, 209, 237, 255};
static fw64ColorRGBA8 secondary_skin_color = {133, 97, 69, 255};

void player_palette_init(PlayerPalette* palette, fw64Engine* engine, fw64Allocator* allocator) {

}

void player_palette_uninit(PlayerPalette* palette, fw64Engine* engine, fw64Allocator* allocator) {
    for (int i = 0; i < PALETTE_TEXTURE_PRIMITIVE_INDEX_COUNT; i++) {
        fw64Image* image = fw64_texture_get_image(&palette->secondary_textures[i]);
        fw64_image_delete(engine->assets, image, allocator);
        fw64_texture_delete(engine->assets, &palette->secondary_textures[i]);
    }
}

static void set_palette(PlayerPalette* palette, fw64ColorRGBA8* hair_color, fw64ColorRGBA8* skin_color, fw64Texture* textures) {
    for (int i = 0; i < HAIR_PRIMITVE_INDEX_COUNT; i++) {
        fw64Material* material = fw64_mesh_get_material_for_primitive(palette->player_mesh, hair_primitive_indices[i]);
        fw64_material_set_color(material, hair_color->r, hair_color->g, hair_color->b);
    }

    for (int i = 0; i < SKIN_PRIMITIVE_INDEX_COUNT; i++) {
        fw64Material* material = fw64_mesh_get_material_for_primitive(palette->player_mesh, skin_primitive_indices[i]);
        fw64_material_set_color(material, skin_color->r, skin_color->g, skin_color->b);
    }
}

void player_palette_activate_primary(PlayerPalette* palette) {
    set_palette(palette, &primary_hair_color, &primary_skin_color, &palette->primary_textures[0]);
}

void player_palette_activate_secondary(PlayerPalette* palette) {
    set_palette(palette, &secondary_hair_color, &secondary_skin_color, &palette->secondary_textures[0]);
}