#include "girl.h"

#include "assets.h"

void girl_init(Girl* girl, fw64Node* node, Player* player, fw64Engine* engine, fw64Allocator* allocator) {
    girl->node = node;
    girl->engine = engine;
    girl->allocator = allocator;
    girl->player = player;

    vec3_set_all(&girl->node->transform.scale, 0.02f);
    quat_set_axis_angle(&girl->node->transform.rotation, 0.0f, 1.0f, 0.0f, M_PI);
    fw64_node_set_mesh(girl->node, fw64_mesh_load(engine->assets, FW64_ASSET_mesh_girl, allocator));
    fw64_node_update(girl->node);
}

void girl_uninit(Girl* girl) {
    fw64_mesh_delete(girl->engine->assets, girl->node->mesh, girl->allocator);
}

void girl_update(Girl* girl) {

}

void girl_draw(Girl* girl) {
    fw64_renderer_draw_static_mesh(girl->engine->renderer, &girl->node->transform, girl->node->mesh);
}