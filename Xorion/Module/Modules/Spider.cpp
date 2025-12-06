#include "Spider.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/LocalPlayer.h"

Spider::Spider() : IModule(0, Category::MOVEMENT, "Climb walls like a spider.") {
	registerFloatSetting("Speed", &speed, 0.6f, 0.1f, 1.f);
	registerBoolSetting("Avoid Overshooting", &dontOvershoot, true);
}

Spider::~Spider() {
}

const char* Spider::getModuleName() {
	return "Spider";
}

void Spider::onTick(GameMode* gm) {
	LocalPlayer* player = Game.getLocalPlayer();
	if (player == nullptr)
		return;

	if (player->isInWater())
		return;

	if (player->isSneaking())
		return;

	// Simplified spider - detect if against wall and climb
	Vec3 playerPos = player->getPos();

	// Check if horizontally colliding (simplified check)
	bool againstWall = false;
	auto aabb = player->getAABBShapeComponent();
	if (aabb != nullptr) {
		Vec3i blockPos = Vec3i(playerPos.floor());
		for (int x = -1; x <= 1; x++) {
			for (int z = -1; z <= 1; z++) {
				if (x == 0 && z == 0) continue;
				Vec3i testPos = blockPos.add(x, 0, z);
				auto block = player->getRegion()->getBlock(testPos);
				if (block != nullptr && block->blockLegacy != nullptr) {
					BlockLegacy* legacy = block->toLegacy();
					// Check if it's a solid block (simple heuristic)
					if (legacy != nullptr) {
						againstWall = true;
						break;
					}
				}
			}
			if (againstWall) break;
		}
	}

	// If against wall and moving forward, climb up
	if (againstWall && player->entityLocation != nullptr) {
		if (player->entityLocation->velocity.y < speed) {
			player->entityLocation->velocity.y = speed;
		}
	}
}