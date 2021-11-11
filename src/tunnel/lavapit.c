#include "lavapit.h"

#include "assets.h"

void tunnel_lavapit_description(SceneDescription* desc) {
    memset(desc, 0, sizeof(SceneDescription));

    desc->index = FW64_ASSET_scene_lavapit;
    desc->init_func = tunnel_lavapit_init;
    desc->update_func = tunnel_lavapit_update;
}

void tunnel_lavapit_init(void* level_arg, fw64Scene* scene, void* data_arg) {

}

void tunnel_lavapit_update(void* level_arg, fw64Scene* scene, void* data_arg) {

}