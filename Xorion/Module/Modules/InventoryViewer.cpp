// Xorion/Module/Modules/InventoryViewer.cpp
#include "InventoryViewer.h"
#include "../../DrawUtils.h"
#include "../../SDK/GameData.h"
#include "../../SDK/C_LocalPlayer.h"
#include "../../Utils/Target.h"
#include "../../Xorion/Friends/FriendList.h"
#include "../../SDK/Inventory.h"

InventoryViewer::InventoryViewer() : IModule(0x0, Category::UTIL, "View nearby entity inventory") {
    registerBoolSetting("players only", &playersOnly, playersOnly);
    registerFloatSetting("scale", &scale, scale, 0.6f, 2.0f);
    registerFloatSetting("x", &x, x, 0.f, 800.f);
    registerFloatSetting("y", &y, y, 0.f, 600.f);
}

InventoryViewer::~InventoryViewer() {}
const char* InventoryViewer::getModuleName() { return "InventoryViewer"; }

void InventoryViewer::findTarget() {
    C_LocalPlayer* lp = g_Data.getLocalPlayer();
    if (!lp) { target = nullptr; return; }

    C_Entity* best = nullptr;
    float bestDist = 6.0f; // view only very nearby to reduce spoof
    g_Data.forEachEntity([&](C_Entity* ent, bool) {
        if (!ent || ent == lp) return;
        if (playersOnly && !ent->isPlayer()) return;
        if (FriendList::findPlayer(ent->getNameTag()->getText())) return;
        if (!Target::isValidTarget(ent)) return;
        float d = (*ent->getPos()).dist(*lp->getPos());
        if (d < bestDist) { bestDist = d; best = ent; }
    });
    target = best;
}

void InventoryViewer::onTick(C_GameMode* gm) {
    if (!g_Data.isInGame()) { target = nullptr; return; }
    findTarget();
}

void InventoryViewer::drawInventory(C_Entity* ent) {
    if (!ent) return;

    // Draw a simple 9x4 grid (like player inventory, not including armor)
    vec2_t base(x, y);
    float cell = 18.0f * scale;
    MC_Color slotCol(60, 60, 60, 140);

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 9; col++) {
            vec2_t p = base.add(vec2_t(col * cell, row * cell));
            DrawUtils::fillRectangleA(p, p.add(vec2_t(cell - 2, cell - 2)), slotCol, 1.0f);
        }
    }

    // Name header
    std::string name = ent->getNameTag() ? ent->getNameTag()->getText() : "Entity";
    DrawUtils::drawText(base.add(vec2_t(0, -14.0f * scale)), &name, MC_Color(255, 255, 255), scale);
}

void InventoryViewer::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
    if (!g_Data.isInGame() || !target) return;
    drawInventory(target);
}
