#include "level.h"

#include "typemap.h"
#include "assets.h"

void level_init(Level* level, fw64Engine* engine) {
    level_base_init(&level->base, engine, FW64_ASSET_scene_simple_scene);
}

void level_update(Level* level){
    level_base_update(&level->base);
}

void level_draw(Level* level) {
    level_base_draw(&level->base);
}

void level_uninit(Level* level) {
    
}