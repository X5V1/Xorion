// Xorion/Module/Modules/AntiEffect.cpp
#include "AntiEffect.h"
#include "../../SDK/GameData.h"
#include "../../SDK/C_LocalPlayer.h"
#include "../../SDK/MobEffect.h"

AntiEffect::AntiEffect() : IModule(0x0, Category::PLAYER, "Remove/ignore negative potion effects") {
    registerBoolSetting("blindness", &clearBlindness, clearBlindness);
    registerBoolSetting("slowness", &clearSlowness, clearSlowness);
    registerBoolSetting("weakness", &clearWeakness, clearWeakness);
    registerBoolSetting("poison", &clearPoison, clearPoison);
    registerBoolSetting("mining fatigue", &clearMiningFatigue, clearMiningFatigue);
}
AntiEffect::~AntiEffect() {}
const char* AntiEffect::getModuleName() { return "AntiEffect"; }

void AntiEffect::purgeEffects() {
    C_LocalPlayer* lp = g_Data.getLocalPlayer();
    if (!lp) return;

    auto effects = lp->getEffectList(); // assume SDK exposes effect list
    if (!effects) return;

    for (auto it = effects->begin(); it != effects->end();) {
        auto id = it->getEffectId(); // MobEffectId
        bool remove = false;
        // Typical Bedrock IDs (may vary by SDK; adjust if needed)
        // Blindness=15, Slowness=2, Weakness=18, Poison=19, MiningFatigue=4
        if (clearBlindness && id == 15) remove = true;
        if (clearSlowness && id == 2) remove = true;
        if (clearWeakness && id == 18) remove = true;
        if (clearPoison && id == 19) remove = true;
        if (clearMiningFatigue && id == 4) remove = true;

        if (remove) it = effects->erase(it);
        else ++it;
    }
}

void AntiEffect::onTick(C_GameMode* gm) {
    if (!g_Data.isInGame()) return;
    purgeEffects();
}
