// Xorion/Module/Modules/DVDLogo.cpp
#include "DVDLogo.h"
#include "../../DrawUtils.h"
#include "../../SDK/GameData.h"

DVDLogo::DVDLogo() : IModule(0x0, Category::VISUAL, "Bouncing DVD logo overlay") {
    registerFloatSetting("speed x", &vx, vx, 0.2f, 4.0f);
    registerFloatSetting("speed y", &vy, vy, 0.2f, 4.0f);
    registerFloatSetting("scale", &scale, scale, 0.5f, 2.0f);
}
DVDLogo::~DVDLogo() {}
const char* DVDLogo::getModuleName() { return "DVDLogo"; }

void DVDLogo::onTick(C_GameMode* gm) {
    // simple bounds (UI space); adjust via DrawUtils::getScreenSize if available
    vec2_t screen = DrawUtils::getScreenSize();
    float maxX = screen.x - (w * scale);
    float maxY = screen.y - (h * scale);

    x += vx;
    y += vy;
    if (x < 0) { x = 0; vx = -vx; }
    if (y < 0) { y = 0; vy = -vy; }
    if (x > maxX) { x = maxX; vx = -vx; }
    if (y > maxY) { y = maxY; vy = -vy; }
}

void DVDLogo::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
    vec2_t pos(x, y);
    vec2_t size(w * scale, h * scale);
    MC_Color col = MC_Color(200, 200, 255, 220);
    DrawUtils::fillRectangleA(pos, pos.add(size), MC_Color(30, 30, 30, 160), 1.0f);
    DrawUtils::drawText(pos.add(vec2_t(10 * scale, 18 * scale)), "DVD", col, scale * 1.2f);
    DrawUtils::drawRectangle(pos, pos.add(size), col, 1.0f);
}
