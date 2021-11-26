#include "shadow.h"

#include "assets.h"
#include "layermap.h"


void shadow_init(Shadow* shadow, fw64Engine* engine, fw64Scene* scene, fw64Transform* target) {
    shadow->engine = engine;
    shadow->scene = scene;
    shadow->target = target;
    fw64_node_init(&shadow->node);

    fw64Image* spritesheet = fw64_image_load(engine->assets, FW64_ASSET_image_shadow, NULL);
    shadow->quad = textured_quad_create_with_image(engine, spritesheet, 0, NULL);
    fw64Material* material = fw64_mesh_get_material_for_primitive(shadow->quad, 0);
    fw64_material_set_texture_frame(material, 0);
    fw64_material_set_shading_mode(material, FW64_SHADING_MODE_DECAL_TEXTURE);
    
    shadow->is_active = 1;
}

void shadow_update(Shadow* shadow) {
    Vec3 vec_down = { 0.0f, -1.0f, 0.0f };

    fw64RaycastHit ray_hit;

    float max_shadow_dist = 20.0f;
    fw64_scene_raycast(shadow->scene, &shadow->target->position, &vec_down, NODE_LAYER_GROUND, &ray_hit);

    if(ray_hit.distance < max_shadow_dist) {
        shadow->is_active = 1;
        shadow->node.transform.position = shadow->target->position;
        shadow->node.transform.position.y -= ray_hit.distance;
        shadow->node.transform.scale.x = 1.0f;
        shadow->node.transform.scale.y = 1.0f;
        shadow->node.transform.scale.z = 1.0f;

        float shadow_scalar = (ray_hit.distance / max_shadow_dist);
        shadow_scalar *= shadow_scalar;
        shadow_scalar = 1.0f - shadow_scalar;
        vec3_scale(&shadow->node.transform.scale, &shadow->node.transform.scale, shadow_scalar);
    }
    else {
        shadow->is_active = 0;
    }
}

void shadow_draw(Shadow* shadow) {
    if (!shadow->is_active)
        return;

    fw64_node_billboard(&shadow->node, fw64_renderer_get_camera(shadow->engine->renderer));
    quat_from_euler(&shadow->node.transform.rotation, 90.0, 0, 0);
    fw64_node_update(&shadow->node);
    fw64_renderer_draw_static_mesh(shadow->engine->renderer, &shadow->node.transform, shadow->quad);
    fw64_renderer_set_depth_testing_enabled(shadow->engine->renderer, 1);
}
