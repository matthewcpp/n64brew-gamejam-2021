#include "sparkle.h"

#include "assets.h"


static void sparkle_reset_time(Sparkle* sparkle);

void sparkle_init(Sparkle* sparkle, fw64Engine* engine) {
    sparkle->engine = engine;
    fw64_node_init(&sparkle->node);

    fw64Image* spritesheet = fw64_image_load_with_options(engine->assets, FW64_ASSET_image_sparkle, FW64_IMAGE_FLAG_DMA_MODE, NULL);
    fw64TexturedQuadParams params;

    fw64_textured_quad_params_init(&params);
    params.image = spritesheet;
    params.is_animated = 1;

    sparkle->quad = fw64_textured_quad_create_with_params(engine, &params, NULL);

    sparkle_reset_time(sparkle);
    sparkle->is_active = 0;
}

void sparkle_update(Sparkle* sparkle) {
    if (!sparkle->is_active) {
        return;
    }

    sparkle->prev_time = sparkle->current_time;
    sparkle->current_time += sparkle->engine->time->time_delta;
    sparkle->frame_time += sparkle->engine->time->time_delta;

    if (sparkle->frame_time > SPARKLE_FRAME_DURATION) {
        sparkle->frame_time -= SPARKLE_FRAME_DURATION;
        sparkle->frame_index += 1;

        if (sparkle->frame_index == SPARKLE_FRAME_COUNT) {
            sparkle->is_active = 0;
            return;
        }

        fw64Material* material = fw64_mesh_get_material_for_primitive(sparkle->quad, 0);
        fw64_material_set_texture_frame(material, sparkle->frame_index);
    }
}

void sparkle_draw(Sparkle* sparkle) {
    if (!sparkle->is_active)
        return;

    fw64_node_billboard(&sparkle->node, fw64_renderer_get_camera(sparkle->engine->renderer));
    fw64_renderer_set_depth_testing_enabled(sparkle->engine->renderer, 0);
    fw64_renderer_draw_static_mesh(sparkle->engine->renderer, &sparkle->node.transform, sparkle->quad);
    fw64_renderer_set_depth_testing_enabled(sparkle->engine->renderer, 1);
}

void sparkle_start(Sparkle* sparkle) {
    sparkle_reset_time(sparkle);
    sparkle->is_active = 1;
}

void sparkle_reset_time(Sparkle* sparkle) {
    sparkle->current_time = 0.0f;
    sparkle->prev_time = 0.0f;
    sparkle->frame_time = 0.0f;
    sparkle->frame_index = 0;
}
