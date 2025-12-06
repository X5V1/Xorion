#include "NoClip.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/GameMode.h"

NoClip::NoClip() : IModule(0x0, Category::MOVEMENT, "Phase through walls on all axis") {
}

NoClip::~NoClip() {
}

const char* NoClip::getModuleName() {
	return ("NoClip");
}

void NoClip::onTick(C_GameMode* gm) {
	// TODO: Entity aabb not directly accessible, use getAABBShapeComponent()
	// gm->player->aabb.upper.y = gm->player->aabb.lower.y - (float)1.8f;
}

void NoClip::onDisable() {
	// TODO: Entity aabb not directly accessible
	// if (g_Data.getLocalPlayer() != nullptr)
	// 	g_Data.getLocalPlayer()->aabb.upper.y = g_Data.getLocalPlayer()->aabb.lower.y + (float)1.8f;
}
