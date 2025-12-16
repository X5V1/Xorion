#include "Jesus.h"
#include "../../../SDK/Entity.h"

Jesus::Jesus() : IModule(0, Category::MOVEMENT, "Walk over water, like Jesus.") {
}

Jesus::~Jesus() {
}

const char* Jesus::getModuleName() {
	return "Jesus";
}

static bool isLava(Block* block) {
	if (block == nullptr || block->blockLegacy == nullptr)
		return false;

	auto material = block->blockLegacy->material;
	return material != nullptr && material->isLiquid && material->isSuperHot;
}

void Jesus::onTick(GameMode* gm) {
	if (gm->player->isSneaking()) return;

	bool inWater = gm->player->isInWater();
	bool inLava = false;

	if (auto region = gm->player->getRegion()) {
		Vec3 pos = gm->player->getPos();
		Vec3i feetPos = Vec3i(pos.floor());
		Block* feetBlock = region->getBlock(feetPos);
		Block* belowBlock = region->getBlock(feetPos.add(0, -1, 0));
		inLava = isLava(feetBlock) || isLava(belowBlock);
	}

	if (inWater || inLava) {
		float lift = inLava ? 0.08f : 0.06f;
		gm->player->entityLocation->velocity.y = lift;
		gm->player->setOnGround(true);
		wasOnLiquid = true;
	} else if (wasOnLiquid) {
		wasOnLiquid = false;
		gm->player->entityLocation->velocity.x *= 1.2f;
		gm->player->entityLocation->velocity.z *= 1.2f;
	}
}
