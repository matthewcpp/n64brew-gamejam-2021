set(game_files
    game.h game.c
    )

if (FW64_PLATFORM_N64)
    list(APPEND game_files ${CMAKE_SOURCE_DIR}/src/main_n64.c)
else()
    list(APPEND game_files ${CMAKE_SOURCE_DIR}/src/main_desktop.cpp)
endif()


add_executable(test_audio ${game_files})

target_link_libraries(test_audio PUBLIC core)


if (FW64_PLATFORM_N64)
    create_n64_rom(TARGET test_audio)
endif()