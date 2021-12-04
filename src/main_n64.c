#include "game.h"
#include "framework64/n64/engine.h"

#include "assets.h"

#include <nusys.h>

Game game;
fw64Engine engine;

#if defined(TV_MODE_PAL)
static void callback_prenmi();
#endif

void nusys_game_tick(int pendingGfx) {
    if (pendingGfx < 1) {
        fw64_n64_engine_update(&engine);
        game_update(&game);
        game_draw(&game);
    }
}

void mainproc(void) {

    #if defined(TV_MODE_PAL)
        osViSetMode(&osViModeTable[OS_VI_FPAL_LAN1]);
        osViSetYScale(0.833);
        nuPreNMIFuncSet((NUScPreNMIFunc)callback_prenmi);
    #elif defined(TV_MODE_MPAL)
        osViSetMode(&osViModeTable[OS_VI_MPAL_LAN1]);
    #endif       

    fw64_n64_engine_init(&engine, FW64_ASSET_COUNT);
    game_init(&game, &engine);
    nuGfxFuncSet((NUGfxFunc)nusys_game_tick);
    nuGfxDisplayOn();
}

#if defined(TV_MODE_PAL)
void callback_prenmi()
{
    nuGfxDisplayOff();
    osViSetYScale(1);
}
#endif
