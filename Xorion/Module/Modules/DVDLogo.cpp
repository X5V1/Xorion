// Xorion/Module/Modules/DVDLogo.cpp
#include "DVDLogo.h"
#include "../../DrawUtils.h"
#include "../../../Memory/GameData.h"
#include "../../../Utils/HMath.h"

DVDLogo::DVDLogo() : IModule(0x0, Category::VISUAL, "Bouncing DVD logo overlay") {
    registerFloatSetting("speed x", &vx, vx, 0.2f, 4.0f);
    registerFloatSetting("speed y", &vy, vy, 0.2f, 4.0f);
    registerFloatSetting("scale", &scale, scale, 0.5f, 2.0f);
}
DVDLogo::~DVDLogo() {}
const char* DVDLogo::getModuleName() { return "DVDLogo"; }

void DVDLogo::onTick(C_GameMode* gm) {
    // simple bounds (UI space) - hardcoded screen size for now
    float screenW = 1920.0f; // TODO: get actual screen size
    float screenH = 1080.0f;
    float maxX = screenW - (w * scale);
    float maxY = screenH - (h * scale);

    x += vx;
    y += vy;
    if (x < 0) { x = 0; vx = -vx; }
    if (y < 0) { y = 0; vy = -vy; }
    if (x > maxX) { x = maxX; vx = -vx; }
    if (y > maxY) { y = maxY; vy = -vy; }
}

void DVDLogo::onPostRender(MinecraftUIRenderContext* renderCtx) {
    Vec2 pos(x, y);
    Vec2 size(w * scale, h * scale);
    MC_Color col = MC_Color(200, 200, 255, 220);
    // TODO: fillRectangleA doesn't exist - disabled background for now
    // DrawUtils::fillRectangle(Vec4(pos.x, pos.y, pos.x + size.x, pos.y + size.y), MC_Color(30, 30, 30, 160), 1.0f);
    Vec2 textPos(x + 10 * scale, y + 18 * scale);
    std::string text = "DVD";
    DrawUtils::drawText(textPos, &text, col, scale * 1.2f);
    Vec2 endPos(x + size.x, y + size.y);
    DrawUtils::fillRectangle(pos, endPos);
}
