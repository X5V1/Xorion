// Xorion/Module/Modules/Breadcrumbs.h
#pragma once
#include "Module.h"
#include <vector>
#include "../../SDK/Vec3.h"

class Breadcrumbs : public IModule {
private:
    std::vector<vec3_t> trail;
    int maxPoints = 200;
    int addEveryNTicks = 2;
    int tickCounter = 0;
    float width = 0.8f;
    MC_Color color = MC_Color(120, 200, 255, 180);

public:
    Breadcrumbs();
    ~Breadcrumbs();

    virtual const char* getModuleName() override;
    virtual void onTick(C_GameMode* gm) override;
    virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
    virtual void onEnable() override;
};
