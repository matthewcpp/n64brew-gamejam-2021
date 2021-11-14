#include "shadow.h"

#include "assets.h"


void shadow_init(Shadow* shadow, fw64Engine* engine) {
    shadow->engine = engine;
    fw64_node_init(&shadow->node);

    fw64Image* spritesheet = fw64_image_load(engine->assets, FW64_ASSET_image_shadow, NULL);
    shadow->quad = textured_quad_create_with_image(engine, spritesheet, 0, NULL);
    fw64Material* material = fw64_mesh_get_material_for_primitive(shadow->quad, 0);
    fw64_material_set_texture_frame(material, 0);
    
    shadow->is_active = 1;
}

void shadow_draw(Shadow* shadow) {
    if (!shadow->is_active)
        return;

    fw64_node_billboard(&shadow->node, fw64_renderer_get_camera(shadow->engine->renderer));
    quat_from_euler(&shadow->node.transform.rotation, 90.0, 0, 0);
    fw64_node_update(&shadow->node);
    //fw64_renderer_set_depth_testing_enabled(shadow->engine->renderer, 0);
    fw64_renderer_draw_static_mesh(shadow->engine->renderer, &shadow->node.transform, shadow->quad);
    fw64_renderer_set_depth_testing_enabled(shadow->engine->renderer, 1);
}
