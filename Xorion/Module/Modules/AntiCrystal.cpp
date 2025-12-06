// Xorion/Module/Modules/AntiCrystal.cpp
#include "AntiCrystal.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/LocalPlayer.h"
#include "../../Utils/Target.h"

AntiCrystal::AntiCrystal() : IModule(0x0, Category::COMBAT, "Defend against end crystals") {
    registerFloatSetting("range", &range, range, 2.0f, 8.0f);
    registerBoolSetting("auto break", &autoBreak, autoBreak);
    registerBoolSetting("dodge", &dodge, dodge);
}
AntiCrystal::~AntiCrystal() {}
const char* AntiCrystal::getModuleName() { return "AntiCrystal"; }

void AntiCrystal::actOnCrystals(GameMode* gm) {
    LocalPlayer* lp = g_Data.getLocalPlayer();
    if (!lp) return;

    Entity* nearest = nullptr;
    float bestDist = range;

    // Use Entity* to avoid LocalPlayer undefined type issues
    Entity* lpEntity = (Entity*)lp;

    g_Data.forEachEntity([&](Entity* ent, bool) {
        if (!ent || ent == lpEntity) return;
        // Bedrock "End Crystal" usually has specific type id/name; heuristic:
        auto nameTag = ent->getNameTag();
        if (!nameTag) return;
        std::string txt = nameTag->getText();
        if (txt.find("crystal") == std::string::npos && txt.find("Crystal") == std::string::npos)
            return;

        float d = ent->getPos()->dist(*lpEntity->getPos());
        if (d < bestDist) { bestDist = d; nearest = ent; }
    });

    if (!nearest) return;

    if (autoBreak) {
    g_Data.getGameMode()->attack(nearest);
    }

    if (dodge) {
        // Basic backward nudge away from crystal
        auto pos = *lpEntity->getPos();
        auto cpos = *nearest->getPos();
        vec3_t dir = pos.sub(cpos);
        if (dir.magnitude() > 0.001f) {
            dir = dir.normalize().mul(0.35f);
            vec3_t newPos = pos.add(dir);
            lpEntity->setPos(newPos);
        }
    }
}

void AntiCrystal::onTick(GameMode* gm) {
    if (!g_Data.isInGame()) return;
    actOnCrystals(gm);
}
