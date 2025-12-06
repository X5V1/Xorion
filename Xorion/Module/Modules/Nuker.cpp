#include "Nuker.h"
#include "../../../SDK/Entity.h"

Nuker::Nuker() : IModule(VK_NUMPAD5, Category::WORLD, "Break multiple blocks at once.") {
	registerIntSetting("Radius", &nukerRadius, nukerRadius, 1, 10);
	registerBoolSetting("Veinminer", &veinMiner, veinMiner);
	registerBoolSetting("Autodestroy", &autodestroy, autodestroy);
}

Nuker::~Nuker() {
}

const char* Nuker::getModuleName() {
	return "Nuker";
}

void Nuker::onTick(GameMode* gm) {
	if (!autodestroy) return;
	// gm->player is Entity*, so getPos() returns Vec3*
	Vec3 playerPos = *gm->player->getPos();
	Vec3i tempPos((int)playerPos.x, (int)playerPos.y, (int)playerPos.z);
	for (int x = -nukerRadius; x < nukerRadius; x++) {
		for (int y = -nukerRadius; y < nukerRadius; y++) {
			for (int z = -nukerRadius; z < nukerRadius; z++) {
				Vec3 currentPos = *gm->player->getPos();
				tempPos.x = (int)currentPos.x + x;
				tempPos.y = (int)currentPos.y + y;
				tempPos.z = (int)currentPos.z + z;
				if (tempPos.y > -64 && gm->player->getRegion()->getBlock(tempPos)->toLegacy()->isSolid) {
					gm->destroyBlock(tempPos, 1);
				}
			}
		}
	}
}
