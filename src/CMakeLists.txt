set(src_files
    animated_material_texture.h animated_material_texture.c
    chase_camera.h chase_camera.c
    enemy.h enemy.c
    fade_effect.h fade_effect.c
    game.h game.c
    moving_platform.h moving_platform.c

    dialogue_window.h dialogue_window.c
    player.h player.c
    player_palette.h player_palette.c
    shadow.h shadow.c
    sparkle.h sparkle.c
    scene_manager.h scene_manager.c
    trigger_box.h trigger_box.c
    ui.h ui.c
    ui_navigation.h ui_navigation.c

    states/playing.h states/playing.c
    states/title.h states/title.c
    states/gamestates.h states/gamestates.c
    states/end.h states/end.c

    tunnel/tunnel_level.h tunnel/tunnel_level.c
    tunnel/girl.h tunnel/girl.c
    tunnel/lavapit.h tunnel/lavapit.c
    tunnel/hallway.h tunnel/hallway.c
    tunnel/firewall.h tunnel/firewall.c
    tunnel/fire_obstacle.h tunnel/fire_obstacle.c
    tunnel/atrium.h tunnel/atrium.c
    tunnel/cutscene.h tunnel/cutscene.c
    )

if (FW64_PLATFORM_N64)
    list(APPEND src_files main_n64.c)
else()
    list(APPEND src_files main_desktop.cpp)
endif()

add_executable(jam ${src_files})

target_link_libraries(jam PUBLIC framework64)
target_include_directories(jam
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../lib/framework64/examples/common
        PRIVATE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/include)

if (FW64_PLATFORM_N64)
    create_n64_rom(TARGET jam)
endif()