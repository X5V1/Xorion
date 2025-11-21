#pragma once
#include "Module.h"
#include "SDK/Vec3i.h"
#include "SDK/GameMode.h"
#include "SDK/MinecraftUIRenderContext.h"

class AutoAnchor : public IModule {
public:
    AutoAnchor();
    ~AutoAnchor();

    const char* getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void onTick(GameMode* gm) override;
    void onPreRender(MinecraftUIRenderContext* renderCtx) override;

private:
    // Settings
    int range = 5;
    bool autoplace = false;
    bool autocharge = false;

    // State
    int delay = 0;
    int prevSlot = -1;
    bool FinishSelect = false;

    // Anchor positions
    std::vector<vec3_ti> anchorList;
};
