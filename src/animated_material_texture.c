#include "animated_material_texture.h"

void animated_material_texture_init(AnimatedMaterialTexture* animated_material, fw64Material* material, float frame_duration){
    animated_material->material = material;
    animated_material->current_frame_time = 0.0f;
    animated_material->frame_duration = frame_duration;
}

void animated_material_texture_update(AnimatedMaterialTexture* animated_material, float time_delta) {
    animated_material->current_frame_time += time_delta;

    if (animated_material->current_frame_time >= animated_material->frame_duration) {
        animated_material->current_frame_time -= animated_material->frame_duration;
        
        fw64Texture* texture = fw64_material_get_texture(animated_material->material);
        int current_frame = fw64_material_get_texture_frame(animated_material->material);
        int total_frame_count = fw64_texture_hslices(texture) * fw64_texture_vslices(texture);

        current_frame += 1;
        if (current_frame >= total_frame_count)
            current_frame = 0;

        fw64_material_set_texture_frame(animated_material->material, current_frame);
    }
}