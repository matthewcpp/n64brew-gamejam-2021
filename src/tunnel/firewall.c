#include "firewall.h"

#include "tunnel_level.h"

#include "assets.h"

#include <string.h>

void tunnel_firewall_description(SceneDescription* desc) {
    memset(desc, 0, sizeof(SceneDescription));

    desc->index = FW64_ASSET_scene_firewall;
    desc->init_func = tunnel_firewall_init;
    desc->update_func = tunnel_firewall_update;
    desc->draw_func = tunnel_firewall_draw;
}

void tunnel_firewall_init(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* tunnel_level = (TunnelLevel*)level_arg;
    Firewall* firewall = (Firewall*)data_arg;
    fw64Allocator* allocator = fw64_scene_get_allocator(scene);

    firewall->flame_image = fw64_image_load_with_options(tunnel_level->engine->assets, FW64_ASSET_image_fire_sprite, FW64_IMAGE_FLAG_DMA_MODE, allocator);

}

void tunnel_firewall_update(void* level_arg, fw64Scene* scene, void* data_arg) {

}

void tunnel_firewall_draw(void* level_arg, fw64Scene* scene, void* data_arg) {

}