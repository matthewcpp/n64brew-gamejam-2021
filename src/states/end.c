#include "end.h"

#include "catherine_title_animation.h"

#define END_SCREEN_ALLOCATOR_SIZE 64 * 1024

static void setup_camera(EndScreen* end_screen);

void end_screen_init(EndScreen* end_screen, fw64Engine* engine, GameStateData* game_state) {
    end_screen->engine = engine;
    end_screen->game_state = game_state;

    ui_navigation_init(&end_screen->ui_navigation, engine->input);
    fw64_bump_allocator_init(&end_screen->allocator, END_SCREEN_ALLOCATOR_SIZE);
    title_animation_init(&end_screen->animation, engine, catherine_title_animation_Tired, &end_screen->allocator.interface);
    setup_camera(end_screen);
}

void end_screen_update(EndScreen* end_screen) {
    title_animation_update(&end_screen->animation);
    ui_navigation_update(&end_screen->ui_navigation);

    if (ui_navigation_accepted(&end_screen->ui_navigation)) {
        end_screen->game_state->transition_state = GAME_STATE_TITLE;
    }
}

void end_screen_draw(EndScreen* end_screen) {
    fw64_renderer_begin(end_screen->engine->renderer, &end_screen->camera, FW64_RENDERER_MODE_TRIANGLES, FW64_RENDERER_FLAG_CLEAR);
    fw64_renderer_draw_animated_mesh(end_screen->engine->renderer, end_screen->animation.mesh, &end_screen->animation.animation_controller, &end_screen->animation.transforms[0]);
    fw64_renderer_end(end_screen->engine->renderer, FW64_RENDERER_FLAG_SWAP);
}

void end_screen_uninit(EndScreen* end_screen) {
    title_animation_uninit(&end_screen->animation, &end_screen->allocator.interface);
    fw64_bump_allocator_uninit(&end_screen->allocator);
}

static void setup_camera(EndScreen* end_screen) {
    fw64_camera_init(&end_screen->camera);
    end_screen->camera.near = 1;
    end_screen->camera.far = 25.0f;
    fw64_camera_update_projection_matrix(&end_screen->camera);

    vec3_set(&end_screen->camera.transform.position, -5.0f, 2.0f, 0.0f);

    Vec3 target = {0.0f, 2.0f, 0.0f};
    Vec3 up = {0.0f, 1.0f, 0.0f};
    fw64_transform_look_at(&end_screen->camera.transform, &target, &up);
    fw64_camera_update_view_matrix(&end_screen->camera);

    quat_set_axis_angle(&end_screen->animation.transforms[0].rotation, 0.0f, 1.0f, 0.0f, -M_PI / 4.0f);
    fw64_transform_update_matrix(&end_screen->animation.transforms[0]);
}