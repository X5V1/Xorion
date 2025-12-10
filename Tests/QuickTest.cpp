// QuickTest.cpp (optional helper used during dev)
#include "GameData.h"
#include "SDK/LocalPlayer.h"
#include "Utils/Logger.h"
#include "SDK/VersionTag.h"

void RunQuickTests() {
    auto lp = g_Data.getLocalPlayer();
    if (!lp) { logF("[QuickTest] LocalPlayer null (%s)", kMinecraftVersion.c_str()); return; }
    try {
        auto name = lp->getName();
        auto pos  = lp->getPos();
        logF("[QuickTest] Name=%s Pos=(%.2f, %.2f, %.2f)", name.c_str(), pos.x, pos.y, pos.z);
    } catch (const std::exception& e) {
        logF("[QuickTest] SDK call failed: %s", e.what());
    }
}
