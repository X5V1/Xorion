#include "ChestESP.h"
#include "../../../Memory/GameData.h"
#include <set>
#include <memory>
#include <algorithm>



ChestESP::ChestESP() : IModule('H', Category::VISUAL, "ESP for chests")
{
}


ChestESP::~ChestESP()
{
}

const char* ChestESP::getModuleName()
{
	return ("ChestESP");
}

void ChestESP::onPostRender(MinecraftUIRenderContext* ctx) {

	if (!g_Data.isInGame() || !g_Data.canUseMoveKeys())
		return;
	//if (listSize < 1000 && listSize > 1) {
	static float rcolors[4];
	if (rcolors[3] < 1) {
		rcolors[0] = 0.2f;
		rcolors[1] = 0.2f;
		rcolors[2] = 1.f;
		rcolors[3] = 1;
	}

	Utils::ColorConvertRGBtoHSV(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]); // premium code right here

	rcolors[0] += 0.0015f;
	if (rcolors[0] >= 1)
		rcolors[0] = 0;

	Utils::ColorConvertHSVtoRGB(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);

	// TODO: ChestESP functionality disabled - SDK issues with chest tracking
	/*
	std::set<std::shared_ptr<AABB>>* chestList = g_Data.getChestList();

	for (auto iter = chestList->begin(); iter != chestList->end(); ++iter){
		DrawUtils::setColor(1.f, 0.3f, 0.3f, 0.2f);
		DrawUtils::drawBox((*iter)->lower, (*iter)->upper, std::max(0.2f, 1 / std::max(1.0f, g_Data.getLocalPlayer()->eyePos0.dist((*iter)->lower)))); // Fancy math to give an illusion of good esp
	}
	*/

}

void ChestESP::onTick(C_GameMode * gm)
{
	// TODO: ChestESP functionality disabled
	/*
	tickTimeout++;
	if (tickTimeout > 60) {
		tickTimeout = 0;
		g_Data.getChestList()->clear();
	}
	*/
}

