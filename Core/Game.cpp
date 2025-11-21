#include "Game.h"
#include "Utils/Utils.h"

GameClass Game;

static bool initialized = false;

static void initGameModuleIfNeeded() {
    if (!initialized) {
        Game.getModule()->ptrBase = Utils::getBase();
        initialized = true;
    }
}

// Call once early in your loader or main init
struct GameInit {
    GameInit() { initGameModuleIfNeeded(); }
} _gameInitOnce;
