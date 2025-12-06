#include "Phase.h"
#include "../../../Memory/GameData.h"

Phase::Phase() : IModule('I', Category::MOVEMENT, "Walk straight through walls.") {
}

Phase::~Phase() {
}

const char* Phase::getModuleName() {
	return "Phase";
}

void Phase::onTick(GameMode* gm) {
	gm->player->getAABBShapeComponent()->aabb.upper.y = gm->player->getAABBShapeComponent()->aabb.lower.y;
}

void Phase::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr)
		g_Data.getLocalPlayer()->getAABBShapeComponent()->aabb.upper.y += 1.8f;
}
