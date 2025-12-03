// Xorion/Module/Modules/TargetHUD.cpp
#include "TargetHUD.h"
#include "../../DrawUtils.h"
#include "../../SDK/C_LocalPlayer.h"
#include "../../Utils/Target.h"
#include "../../Xorion/Friends/FriendList.h"

TargetHUD::TargetHUD() : IModule(0x0, Category::COMBAT, "Display info about your current target") {
    registerBoolSetting("players only", &onlyPlayers, onlyPlayers);
    registerBoolSetting("show armor", &showArmor, showArmor);
    registerBoolSetting("show health", &showHealth, showHealth);
    registerBoolSetting("show distance", &showDistance, showDistance);
    registerFloatSetting("scale", &scale, scale, 0.6f, 2.0f);
    registerFloatSetting("x", &x, x, 0.0f, 500.0f);
    registerFloatSetting("y", &y, y, 0.0f, 300.0f);
    registerIntSetting("hold ticks", &holdTicks, holdTicks, 0, 40);
    registerIntSetting("fade ticks", &fadeTicks, fadeTicks, 0, 40);
}

TargetHUD::~TargetHUD() {}

const char* TargetHUD::getModuleName() { return "TargetHUD"; }

C_Entity* TargetHUD::pickTarget() {
    C_LocalPlayer* lp = g_Data.getLocalPlayer();
    if (!lp) return nullptr;

    C_Entity* best = nullptr;
    float bestDist = 1000.0f;

    g_Data.forEachEntity([&](C_Entity* ent, bool) {
        if (!ent || ent == lp) return;
        if (FriendList::findPlayer(ent->getNameTag()->getText())) return;
        if (!Target::isValidTarget(ent)) return;
        if (onlyPlayers && !ent->isPlayer()) return;

        float dist = (*ent->getPos()).dist(*lp->getPos());
        if (dist < bestDist) { bestDist = dist; best = ent; }
    });

    return best;
}

void TargetHUD::onTick(C_GameMode* gm) {
    if (!g_Data.isInGame()) { lastTarget = nullptr; visibleTicks = 0; return; }
    C_Entity* t = pickTarget();
    if (t) { lastTarget = t; visibleTicks = holdTicks + fadeTicks; }
    else if (visibleTicks > 0) visibleTicks--;
}

void TargetHUD::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
    if (!g_Data.isInGame() || !lastTarget || visibleTicks <= 0) return;

    auto name = lastTarget->getNameTag();
    float healthVal = lastTarget->getHealth();
    float maxHealthVal = lastTarget->getMaxHealth();
    float dist = (*lastTarget->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

    float alpha = 1.0f;
    if (visibleTicks < fadeTicks) alpha = visibleTicks / (float)fadeTicks;

    vec2_t base(x, y);
    vec2_t size(160.0f * scale, 60.0f * scale);
    MC_Color bg(20, 20, 20, (int)(alpha * 160));
    MC_Color accent = ColorUtil::rainbow(4, 0.8f, 1.0f, (int)(alpha * 255));

    DrawUtils::fillRectangleA(base, base.add(size), bg, 1.0f);

    std::string n = name ? name->getText() : "Unknown";
    DrawUtils::drawText(base.add(vec2_t(6.0f * scale, 6.0f * scale)), &n, MC_Color(255, 255, 255, (int)(alpha * 255)), scale);

    if (showHealth && maxHealthVal > 0.1f) {
        float pct = std::max(0.0f, std::min(1.0f, healthVal / maxHealthVal));
        vec2_t hbPos = base.add(vec2_t(6.0f * scale, 22.0f * scale));
        vec2_t hbSize(148.0f * scale, 8.0f * scale);
        DrawUtils::fillRectangleA(hbPos, hbPos.add(hbSize), MC_Color(50, 50, 50, (int)(alpha * 140)), 1.0f);
        DrawUtils::fillRectangleA(hbPos, hbPos.add(vec2_t(hbSize.x * pct, hbSize.y)), MC_Color(80, 220, 120, (int)(alpha * 200)), 1.0f);
    }

    if (showDistance) {
        char buf[64]; sprintf_s(buf, "Dist: %.1f", dist);
        DrawUtils::drawText(base.add(vec2_t(6.0f * scale, 36.0f * scale)), buf, MC_Color(200, 200, 200, (int)(alpha * 200)), scale);
    }

    if (showArmor) {
        vec2_t aBase = base.add(vec2_t(6.0f * scale, 48.0f * scale));
        for (int i = 0; i < 4; i++) {
            vec2_t slotPos = aBase.add(vec2_t(i * 18.0f * scale, 0));
            DrawUtils::fillRectangleA(slotPos, slotPos.add(vec2_t(16.0f * scale, 10.0f * scale)), MC_Color(80, 80, 80, (int)(alpha * 160)), 1.0f);
        }
    }

    DrawUtils::drawRectangle(base, base.add(size), accent, 1.0f);
}
