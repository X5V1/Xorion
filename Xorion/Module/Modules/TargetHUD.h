// Xorion/Module/Modules/TargetHUD.h
#pragma once
#include "Module.h"
#include "../../DrawUtils.h"
#include "../../SDK/C_Entity.h"
#include "../../SDK/GameData.h"
#include "../../Utils/ColorUtil.h"

class TargetHUD : public IModule {
private:
    bool showArmor = true;
    bool showHealth = true;
    bool showDistance = true;
    bool onlyPlayers = true;
    float scale = 1.0f;
    float x = 10.0f;
    float y = 10.0f;

    C_Entity* lastTarget = nullptr;
    int holdTicks = 10;
    int fadeTicks = 8;
    int visibleTicks = 0;

    C_Entity* pickTarget();

public:
    TargetHUD();
    ~TargetHUD();

    virtual const char* getModuleName() override;
    virtual void onTick(C_GameMode* gm) override;
    virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
};
