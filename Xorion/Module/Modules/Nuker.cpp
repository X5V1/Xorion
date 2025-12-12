#include "Nuker.h"
#include <cmath>
#include "../../../Memory/GameData.h"
#include "../../../SDK/Entity.h"

Nuker::Nuker() : IModule(VK_NUMPAD5, Category::WORLD, "Break multiple blocks at once.") {
	registerIntSetting("Radius", &nukerRadius, nukerRadius, 1, 10);
	registerIntSetting("Delay", &delay, delay, 0, 10);
	registerBoolSetting("Veinminer", &veinMiner, veinMiner);
	registerBoolSetting("Autodestroy", &autodestroy, autodestroy);
}

Nuker::~Nuker() {
}

const char* Nuker::getModuleName() {
	return "Nuker";
}

void Nuker::onTick(GameMode* gm) {
	if (!g_Data.canUseMoveKeys() || gm == nullptr || gm->player == nullptr)
		return;

	if (delay > 0) {
		if (++tickTimer <= delay)
			return;
		tickTimer = 0;
	}

	auto player = gm->player;
	if (player->level == nullptr)
		return;

	Vec3i origin{};
	if (autodestroy) {
		Vec3 playerPos = *player->getPos();
		origin = Vec3i((int)playerPos.x, (int)playerPos.y, (int)playerPos.z);
	} else {
		if (!g_Data.isLeftClickDown())
			return;
		origin = player->level->hitResult.blockPos;
	}

	auto targetBlock = player->getRegion()->getBlock(origin)->toLegacy();
	if (targetBlock == nullptr || targetBlock->blockId == 0)
		return;

	const int targetId = targetBlock->blockId;
	const int radiusSq = nukerRadius * nukerRadius;

	for (int x = -nukerRadius; x <= nukerRadius; x++) {
		const int xSq = x * x;
		const int yMax = static_cast<int>(std::sqrt(static_cast<float>(radiusSq - xSq)));
		for (int y = -yMax; y <= yMax; y++) {
			const int remaining = radiusSq - xSq - (y * y);
			if (remaining < 0)
				continue;
			const int zMax = static_cast<int>(std::sqrt(static_cast<float>(remaining)));
			for (int z = -zMax; z <= zMax; z++) {

				Vec3i blockPos = origin.add(x, y, z);
				if (blockPos.y <= -64)
					continue;

				auto blockLegacy = player->getRegion()->getBlock(blockPos)->toLegacy();
				if (blockLegacy == nullptr || blockLegacy->blockId == 0)
					continue;

				if (veinMiner && blockLegacy->blockId != targetId)
					continue;

				gm->destroyBlock(blockPos, 1);
			}
		}
	}
	player->swingArm();
}
