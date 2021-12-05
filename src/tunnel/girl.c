#include "girl.h"

#include "assets.h"


void girl_init(Girl* girl, fw64Engine* engine, fw64Scene* scene, int node_index) {
    girl->node = fw64_scene_get_node(scene, node_index);
    girl->engine = engine;
    girl->scene = scene;

    vec3_set_all(&girl->node->transform.scale, 0.02f);
    quat_set_axis_angle(&girl->node->transform.rotation, 0.0f, 1.0f, 0.0f, M_PI);
    fw64_node_set_mesh(girl->node, fw64_mesh_load(engine->assets, FW64_ASSET_mesh_girl, fw64_scene_get_allocator(girl->scene)));

    fw64RaycastHit hit;
    Vec3 origin = girl->node->transform.position;
    origin.y += 5;
    Vec3 direction = {0.0, -1.0f, 0.0f};
    if (fw64_scene_raycast(girl->scene, &origin, &direction, 2, &hit)) {
        girl->node->transform.position = hit.point;
    }

    fw64_node_update(girl->node);
}

void girl_uninit(Girl* girl) {
    fw64_mesh_delete(girl->engine->assets, girl->node->mesh, fw64_scene_get_allocator(girl->scene));
}

void girl_update(Girl* girl) {

}

void girl_draw(Girl* girl) {
    fw64_renderer_draw_static_mesh(girl->engine->renderer, &girl->node->transform, girl->node->mesh);
}