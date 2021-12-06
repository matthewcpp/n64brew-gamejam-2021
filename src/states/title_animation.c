#include "title_animation.h"

#include "assets.h"
#include "catherine_title_animation.h"

void title_animation_init(TitleAnimation* animation, fw64Engine* engine, int inital_animation, fw64Allocator* allocator){
    animation->engine = engine;
    animation->mesh = fw64_mesh_load(engine->assets, FW64_ASSET_mesh_catherine, allocator);
    animation->animation_data = fw64_animation_data_load(engine->assets, FW64_ASSET_animation_data_catherine_title, allocator);
    fw64_animation_controller_init(&animation->animation_controller, animation->animation_data, inital_animation, allocator);
    fw64_animation_controller_play(&animation->animation_controller);

    fw64_transform_init(&animation->transforms[0]);
    vec3_set(&animation->transforms[0].position, 0.0f, 0.0f, 1.5f);
    vec3_set_all(&animation->transforms[0].scale, 0.02f);
    fw64_transform_update_matrix(&animation->transforms[0]);

    fw64_transform_init(&animation->transforms[1]);
    vec3_set(&animation->transforms[1].position, 0.0f, 0.0f, -1.5f);
    quat_set_axis_angle(&animation->transforms[1].rotation, 0.0f, 1.0f, 0.0f, M_PI);
    vec3_set_all(&animation->transforms[1].scale, 0.02f);
    fw64_transform_update_matrix(&animation->transforms[1]);
}

void title_animation_uninit(TitleAnimation* animation, fw64Allocator* allocator){
    fw64_animation_controller_uninit(&animation->animation_controller, allocator);
    fw64_mesh_delete(animation->engine->assets, animation->mesh, allocator);
    fw64_animation_data_delete(animation->animation_data, allocator);
}

void title_animation_update(TitleAnimation* animation) {
    fw64_animation_controller_update(&animation->animation_controller, animation->engine->time->time_delta);
}