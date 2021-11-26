#pragma once

#include "scene_manager.h"

typedef struct {
    fw64Image* flame_image;
    fw64Mesh* fire_mesh;
} Firewall;

void tunnel_firewall_description(SceneDescription* desc);
void tunnel_firewall_init(void* level_arg, fw64Scene* scene, void* data_arg);
void tunnel_firewall_update(void* level_arg, fw64Scene* scene, void* data_arg);
void tunnel_firewall_draw(void* level_arg, fw64Scene* scene, void* data_arg);