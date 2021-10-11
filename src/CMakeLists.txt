set(src_files
    game.h game.c)

if (FW64_PLATFORM_N64)
    list(APPEND src_files main_n64.c)
else()
    list(APPEND src_files main_desktop.cpp)
endif()

add_executable(jam ${src_files})

target_link_libraries(jam PUBLIC framework64)
target_include_directories(jam
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)

if (FW64_PLATFORM_N64)
    create_n64_rom(TARGET jam)
endif()