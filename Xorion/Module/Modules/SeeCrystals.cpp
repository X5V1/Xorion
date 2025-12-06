#include "SeeCrystals.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/Entity.h"
#include "../../../SDK/ClientInstance.h"
#include "../../../SDK/Level.h"
// #include "../../../Xorion/Render/RenderUtils.h" // TODO: RenderUtils.h not found in expected location
#include "../../../Utils/Logger.h"

namespace {
    inline bool isEndCrystal(Entity* e) {
        if (!e) return false;
        // Replace with verified End Crystal type ID for 1.21.123
        // TODO: isEndCrystal() method not available
        return e->getEntityTypeId() == 71;
    }
}

SeeCrystals::SeeCrystals()
    : IModule(0x0, Category::VISUAL, "Render all End Crystals as purple and visible through walls") {}

void SeeCrystals::onEnable() {
    m_crystals.clear();
    rebuildCrystalCache();
    logF("[SeeCrystals] Enabled. Tint: #7418D6, ThroughWalls: %s", m_throughWalls ? "true" : "false");
}

void SeeCrystals::onDisable() {
    m_crystals.clear();
    logF("[SeeCrystals] Disabled");
}

void SeeCrystals::rebuildCrystalCache() {
    m_crystals.clear();
    auto lp = g_Data.getLocalPlayer();
    if (!lp || !lp->level) return;

    Game.forEachEntity([&](Entity* ent, bool /*isLocal*/) {
        if (isEndCrystal(ent)) {
            m_crystals.emplace_back(ent);
        }
    });
}

// TODO: onLevelRender uses getD3DDevice and RenderUtils methods not available
void SeeCrystals::onLevelRender() {
    // Stub implementation - D3D device access not available in 1.21.123
    return;
}

/*
// Original implementation - commented out due to API changes
void SeeCrystals::onLevelRender_ORIGINAL() {
    if (!this->isEnabled()) return;

    auto ci = Game.getClientInstance();
    if (!ci || !ci->levelRenderer) return;

    auto dev     = ci->getD3DDevice();        // adapt to your SDK
    auto devCtx  = ci->getD3DDeviceContext(); // adapt to your SDK

    rebuildCrystalCache();

    for (auto* ent : m_crystals) {
        if (!ent) continue;
        renderCrystalNormal(ent, dev, devCtx);
        if (m_throughWalls) {
            renderCrystalOverlay(ent, dev, devCtx);
        }
    }
}

void SeeCrystals::renderCrystalNormal(Entity* ent, ID3D11Device* dev, ID3D11DeviceContext* ctx) {
    RenderUtils::StateGuard guard;
    if (!RenderUtils::beginState(guard, ctx)) return;

    RenderUtils::setDepthTest(true, dev, ctx);
    RenderUtils::setDepthWrite(true, dev, ctx);
    RenderUtils::setBlendMode(RenderUtils::BlendMode::Alpha, dev, ctx);

    RenderUtils::bindUnlitTintShader(ctx);
    RenderUtils::setTintColor(ctx, m_color);

    RenderUtils::drawEntityMesh(ent, ctx);

    RenderUtils::endState(guard, ctx);
}

void SeeCrystals::renderCrystalOverlay(Entity* ent, ID3D11Device* dev, ID3D11DeviceContext* ctx) {
    RenderUtils::StateGuard guard;
    if (!RenderUtils::beginState(guard, ctx)) return;

    RenderUtils::setDepthTest(false, dev, ctx);
    RenderUtils::setDepthWrite(false, dev, ctx);
    RenderUtils::setBlendMode(RenderUtils::BlendMode::Alpha, dev, ctx);

    RenderUtils::bindUnlitTintShader(ctx);
    RenderUtils::setTintColor(ctx, m_color);

    RenderUtils::drawEntityMesh(ent, ctx);

    RenderUtils::endState(guard, ctx);
}
*/
