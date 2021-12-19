set(src_files
    animated_material_texture.h animated_material_texture.c
    chase_camera.h chase_camera.c
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
    states/title_animation.h states/title_animation.c

    tunnel/tunnel_level.h tunnel/tunnel_level.c
    tunnel/girl.h tunnel/girl.c
    tunnel/lavapit.h tunnel/lavapit.c
    tunnel/hallway.h tunnel/hallway.c
    tunnel/firewall.h tunnel/firewall.c
    tunnel/fire_obstacle.h tunnel/fire_obstacle.c
    tunnel/atrium.h tunnel/atrium.c
    tunnel/cutscene.h tunnel/cutscene.c
    )

create_game(TARGET game SOURCES ${src_files})