#pragma once
#include "Module.h"
#include <vector>
#include "SDK/Vec3i.h"
#include "SDK/GameMode.h"
#include "SDK/MinecraftUIRenderContext.h"

class BedAura : public IModule {
public:
    // Settings
    int range = 5;
    bool autoplace = true;
    bool autoDetonateNearby = true;
    bool dimensionGuard = true;
    int cycleDelay = 5;

    bool renderBeds = true;
    float renderAlpha = 0.4f;

    // State
    int delay = 0;
    int prevSlot = -1;
    bool restoreSlotPending = false;
    int slotRestoreCountdown = 0;

    std::vector<vec3_ti> bedPositions;

    BedAura();
    ~BedAura();

    const char* getModuleName() override;

    // Hooks
    void onEnable() override;
    void onDisable() override;
    void onTick(GameMode* gm) override;
    void onPreRender(MinecraftUIRenderContext* ctx) override;
};
