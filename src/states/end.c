#include "end.h"

#include "assets.h"
#include "catherine_title_animation.h"

#include <stdio.h>
#include <string.h>

#define END_SCREEN_ALLOCATOR_SIZE 128 * 1024

static void setup_camera(EndScreen* end_screen);

void end_screen_init(EndScreen* end_screen, fw64Engine* engine, GameStateData* game_state) {
    end_screen->engine = engine;
    end_screen->game_state = game_state;

    
    int minutes = game_state->seconds_count / 60;
    int seconds = game_state->seconds_count % 60;
    sprintf(end_screen->time_str, "%02d\"%02d'", minutes, seconds);
    sprintf(end_screen->death_str, "%d", game_state->death_count);

    fw64_bump_allocator_init(&end_screen->allocator, END_SCREEN_ALLOCATOR_SIZE);
    fw64Allocator* allocator = &end_screen->allocator.interface;

    end_screen->title_font = fw64_font_load(engine->assets, FW64_ASSET_font_end_font_title, allocator);
    end_screen->text_font = fw64_font_load(engine->assets, FW64_ASSET_font_end_font_text, allocator);

    end_screen->measurements[0] = fw64_font_measure_text(end_screen->title_font, "area");
    end_screen->measurements[1] = fw64_font_measure_text(end_screen->title_font, "clear");
    end_screen->measurements[2] = fw64_font_measure_text(end_screen->text_font, "time");
    end_screen->measurements[3] = fw64_font_measure_text(end_screen->text_font, end_screen->time_str);
    end_screen->measurements[4] = fw64_font_measure_text(end_screen->text_font, "deaths");
    end_screen->measurements[5] = fw64_font_measure_text(end_screen->text_font, end_screen->death_str);

    ui_navigation_init(&end_screen->ui_navigation, engine->input);
    
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
    IVec2 screen_size;
    fw64_renderer_get_screen_size(end_screen->engine->renderer, &screen_size);
    fw64Renderer* renderer = end_screen->engine->renderer;
    int title_font_size = fw64_font_size(end_screen->title_font);
    int text_font_size = fw64_font_size(end_screen->text_font);

    fw64_renderer_begin(renderer, &end_screen->camera, FW64_RENDERER_MODE_TRIANGLES, FW64_RENDERER_FLAG_CLEAR);
    fw64_renderer_draw_animated_mesh(renderer, end_screen->animation.mesh, &end_screen->animation.animation_controller, &end_screen->animation.transforms[0]);
    fw64_renderer_end(renderer, FW64_RENDERER_FLAG_NOSWAP);

    fw64_renderer_begin(renderer, &end_screen->camera, FW64_RENDERER_MODE_ORTHO2D, FW64_RENDERER_FLAG_NOCLEAR);
    int draw_y = 20;
    int draw_x = (screen_size.x / 2) - (end_screen->measurements[0].x / 2);

    fw64_renderer_draw_text(renderer, end_screen->title_font, draw_x, draw_y, "area");

    draw_y += title_font_size + 5;
    draw_x = (screen_size.x / 2) - (end_screen->measurements[1].x / 2);

    fw64_renderer_draw_text(renderer, end_screen->title_font, draw_x, draw_y, "clear");

    draw_y += title_font_size + 20;
    draw_x = (screen_size.x / 2) - (end_screen->measurements[2].x / 2);
    fw64_renderer_draw_text(renderer, end_screen->text_font, draw_x, draw_y, "time");

    draw_y += text_font_size + 5;
    draw_x = (screen_size.x / 2) - (end_screen->measurements[3].x / 2);
    fw64_renderer_draw_text(renderer, end_screen->text_font, draw_x, draw_y, end_screen->time_str);

    draw_y += text_font_size + 10;
    draw_x = (screen_size.x / 2) - (end_screen->measurements[4].x / 2);
    fw64_renderer_draw_text(renderer, end_screen->text_font, draw_x, draw_y, "deaths");

    draw_y += text_font_size + 5;
    draw_x = (screen_size.x / 2) - (end_screen->measurements[5].x / 2);
    fw64_renderer_draw_text(renderer, end_screen->text_font, draw_x, draw_y, end_screen->death_str);


    fw64_renderer_end(end_screen->engine->renderer, FW64_RENDERER_FLAG_SWAP);
}

void end_screen_uninit(EndScreen* end_screen) {
    fw64Allocator* allocator = &end_screen->allocator.interface;
    
    fw64_font_delete(end_screen->engine->assets, end_screen->title_font, allocator);
    fw64_font_delete(end_screen->engine->assets, end_screen->text_font, allocator);
    title_animation_uninit(&end_screen->animation, allocator);

    fw64_bump_allocator_uninit(&end_screen->allocator);
}

static void setup_camera(EndScreen* end_screen) {
    fw64_camera_init(&end_screen->camera);
    end_screen->camera.near = 1;
    end_screen->camera.far = 25.0f;
    fw64_camera_update_projection_matrix(&end_screen->camera);

    vec3_set(&end_screen->camera.transform.position, 0.0f, 2.0f, 8.0f);

    Vec3 target = {0.0f, 2.0f, 0.0f};
    Vec3 up = {0.0f, 1.0f, 0.0f};
    fw64_transform_look_at(&end_screen->camera.transform, &target, &up);
    fw64_camera_update_view_matrix(&end_screen->camera);

    quat_set_axis_angle(&end_screen->animation.transforms[0].rotation, 0.0f, 1.0f, 0.0f, M_PI / 4.0f);
    fw64_transform_update_matrix(&end_screen->animation.transforms[0]);
}