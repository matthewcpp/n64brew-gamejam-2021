set(FW64_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib/framework64)

set(core_files
    chase_camera.h chase_camera.c
    level1.h level1.c
    player.h player.c
    ${FW64_DIR}/examples/common/flame.h ${FW64_DIR}/examples/common/flame.c
)
set(game_files
    game.h game.c
    )

if (FW64_PLATFORM_N64)
    list(APPEND game_files main_n64.c)
else()
    list(APPEND game_files main_desktop.cpp)
endif()

add_library(core STATIC ${core_files})

target_include_directories(core
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lib/framework64/examples/common
        PUBLIC ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)

target_link_libraries(core PUBLIC framework64)

add_executable(jam ${game_files})

target_link_libraries(jam PUBLIC core framework64)


if (FW64_PLATFORM_N64)
    create_n64_rom(TARGET jam)
endif()