// Xorion/Module/Modules/Watermark.cpp
#include "Watermark.h"
#include "../../DrawUtils.h"
#include "../../Utils/ColorUtil.h"
#include "../../../SDK/Vec2.h"

Watermark::Watermark() : IModule(0x0, Category::VISUAL, "Client watermark overlay") {
    registerFloatSetting("x", &x, x, 0.f, 1000.f);
    registerFloatSetting("y", &y, y, 0.f, 1000.f);
    registerFloatSetting("scale", &scale, scale, 0.5f, 3.0f);
    registerBoolSetting("rainbow", &rainbow, rainbow);
}
Watermark::~Watermark() {}
const char* Watermark::getModuleName() { return "Watermark"; }

void Watermark::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	MC_Color col = rainbow ? ColorUtil::rainbow(6, 1.0f, 1.0f, 255) : MC_Color(255, 255, 255, 255);
	Vec2 pos(x, y);
	std::string textStr = text;
	DrawUtils::drawText(pos, &textStr, col, scale);
}