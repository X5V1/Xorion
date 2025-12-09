// GameDataChecks.cpp
#include "GameDataChecks.h"
#include "GameData.h"
#include "Utils/Logger.h"

void VerifyGameData() {
    auto lp = g_Data.getLocalPlayer();
    auto ci = g_Data.getClientInstance();
    auto gm = g_Data.getGameMode();

    logF("[GameData] Pointers for 1.21.124 -> LocalPlayer=%p ClientInstance=%p GameMode=%p", lp, ci, gm);
    if (!lp || !ci || !gm) {
        logF("[GameData] One or more critical pointers are null. Offsets likely changed in 1.21.124.");
    }
}
