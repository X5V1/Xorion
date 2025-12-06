#include "Step.h"
#include "../../../Memory/GameData.h"

Step::Step() : IModule(0, Category::MOVEMENT, "Increases your step height.") {
	registerFloatSetting("Height", &height, height, 1.f, 10.f);
}

Step::~Step() {
}

const char* Step::getModuleName() {
	return "Step";
}

void Step::onTick(GameMode* gm) {
	gm->player->getMaxAutoStepComponent()->stepHeight = height;
}
void Step::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr)
		g_Data.getLocalPlayer()->getMaxAutoStepComponent()->stepHeight = 0.563f;
}
