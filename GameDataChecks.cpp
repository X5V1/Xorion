// GameDataChecks.cpp
#include "GameDataChecks.h"
#include "GameData.h"
#include "Utils/Logger.h"
#include "SDK/VersionTag.h"

void VerifyGameData() {
    auto lp = g_Data.getLocalPlayer();
    auto ci = g_Data.getClientInstance();
    auto gm = g_Data.getGameMode();

    logF("[GameData] Pointers for %s -> LocalPlayer=%p ClientInstance=%p GameMode=%p", kMinecraftVersion.c_str(), lp, ci, gm);
    if (!lp || !ci || !gm) {
        logF("[GameData] One or more critical pointers are null. Offsets likely changed in %s.", kMinecraftVersion.c_str());
    }
}
