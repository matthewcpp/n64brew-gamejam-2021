#pragma once


#include "scene_manager.h"
#include "fire_obstacle.h"
#include "animated_material_texture.h"

#define ROLL_OBSTACLE_COUNT 1

typedef struct {
    fw64Image* flame_image;
    fw64Mesh* flame_mesh;
    AnimatedMaterialTexture animated_material;
    FireRollingObstacle roll_obstacles[ROLL_OBSTACLE_COUNT];
    FireObstacleNodePool node_pool;
} Firewall;

#ifdef __cplusplus
extern "C" {
#endif

void tunnel_firewall_description(SceneDescription* desc);
void tunnel_firewall_init(void* level_arg, fw64Scene* scene, void* data_arg);
void tunnel_firewall_update(void* level_arg, fw64Scene* scene, void* data_arg);
void tunnel_firewall_draw(void* level_arg, fw64Scene* scene, void* data_arg);

#ifdef __cplusplus
}
#endif