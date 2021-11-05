#include "level.h"

#include "typemap.h"
#include "assets.h"
#include "scene_playground.h"

void level_init(Level* level, fw64Engine* engine) {
    level_base_init(&level->base, engine);
    level_base_setup_player(&level->base, FW64_scene_playground_node_Player_Start);
}

void level_update(Level* level){
    level_base_update(&level->base);
}

void level_draw(Level* level) {
    level_base_draw(&level->base);
}

void level_uninit(Level* level) {
    
}