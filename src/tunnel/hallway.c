#include "hallway.h"

#include "tunnel_level.h"
#include "scene_hallway.h"
#include "assets.h"

#include <string.h>

void tunnel_hallway_init(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    SceneData* data = (SceneData*)data_arg;

}

void tunnel_hallway_update(void* level_arg, fw64Scene* scene, void* data_arg) {
    TunnelLevel* level = (TunnelLevel*)level_arg;
    SceneData* data = (SceneData*)data_arg;
}

void tunnel_hallway_description(SceneDescription* desc) {
    memset(desc, 0, sizeof(SceneDescription));

    desc->index = FW64_ASSET_scene_hallway;
    desc->init_func = tunnel_hallway_init;
    desc->update_func = tunnel_hallway_update;
}
