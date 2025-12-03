// Xorion/Module/Modules/Breadcrumbs.cpp
#include "Breadcrumbs.h"
#include "../../DrawUtils.h"
#include "../../SDK/C_LocalPlayer.h"

Breadcrumbs::Breadcrumbs() : IModule(0x0, Category::VISUAL, "Render a trail behind you") {
    registerIntSetting("max points", &maxPoints, maxPoints, 20, 1000);
    registerIntSetting("add interval", &addEveryNTicks, addEveryNTicks, 1, 10);
    registerFloatSetting("width", &width, width, 0.2f, 3.0f);
}

Breadcrumbs::~Breadcrumbs() {}

const char* Breadcrumbs::getModuleName() { return "Breadcrumbs"; }

void Breadcrumbs::onEnable() { trail.clear(); tickCounter = 0; }

void Breadcrumbs::onTick(C_GameMode* gm) {
    if (!g_Data.isInGame()) { trail.clear(); return; }
    C_LocalPlayer* lp = g_Data.getLocalPlayer();
    if (!lp) return;

    tickCounter++;
    if (tickCounter % addEveryNTicks == 0) {
        vec3_t p = *lp->getPos();
        trail.push_back(p);
        if ((int)trail.size() > maxPoints)
            trail.erase(trail.begin(), trail.begin() + ((int)trail.size() - maxPoints));
    }
}

void Breadcrumbs::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
    if (trail.size() < 2) return;
    for (size_t i = 1; i < trail.size(); i++) {
        DrawUtils::drawLine3d(trail[i - 1], trail[i], color, width);
    }
}
