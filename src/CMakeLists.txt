set(src_files
    chase_camera.h chase_camera.c
    enemy.h enemy.c
    game.h game.c
    player.h player.c
    sparkle.h sparkle.c
    level_base.h level_base.c
    ui.h ui.c
    trigger_box.h trigger_box.c

    initial/level.h initial/level.c

    tunnel/tunnel_level.h tunnel/tunnel_level.c
    tunnel/key.h tunnel/key.c
    tunnel/hallway.h tunnel/hallway.c
    tunnel/atrium.h tunnel/atrium.c
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