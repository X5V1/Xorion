#include "Tower.h"

#include "../../DrawUtils.h"

Tower::Tower() : IModule(0, Category::WORLD, "Like scaffold, but vertically and a lot faster.") {
	registerFloatSetting("Motion", &motion, motion, 0.3f, 1.f);
}

Tower::~Tower() {
}

const char* Tower::getModuleName() {
	return "Tower";
}

bool Tower::tryTower(Vec3 blockBelow) {
	GameSettingsInput* input = Game.getClientInstance()->getGameSettingsInput();

	if (input == nullptr)
		return false;

	blockBelow = blockBelow.floor();

	DrawUtils::drawBox(blockBelow, Vec3(blockBelow).add(1), 0.4f);

	Block* block = g_Data.getLocalPlayer()->getRegion()->getBlock(Vec3i(blockBelow));
	BlockLegacy* blockLegacy = (block->blockLegacy);
	if (!blockLegacy->isSolid) {
		Vec3i blok(blockBelow);

		// Find neighbour
		static std::vector<Vec3i*> checklist;
		if (checklist.empty()) {
			checklist.push_back(new Vec3i(0, -1, 0));
			checklist.push_back(new Vec3i(0, 1, 0));

			checklist.push_back(new Vec3i(0, 0, -1));
			checklist.push_back(new Vec3i(0, 0, 1));

			checklist.push_back(new Vec3i(-1, 0, 0));
			checklist.push_back(new Vec3i(1, 0, 0));
		}
		bool foundCandidate = false;
		int i = 0;
		for (auto current : checklist) {
			Vec3i calc = blok.sub(*current);
			if (((g_Data.getLocalPlayer()->getRegion()->getBlock(calc)->blockLegacy))->isSolid) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate && GameData::isKeyDown(*input->spaceBarKey)) {
			Vec3 moveVec;
			moveVec.x = g_Data.getLocalPlayer()->entityLocation->velocity.x;
			moveVec.y = motion;
			moveVec.z = g_Data.getLocalPlayer()->entityLocation->velocity.z;
			g_Data.getLocalPlayer()->lerpMotion(moveVec);
			bool idk = true;
			Game.getGameMode()->buildBlock(&blok, i, idk);

			return true;
		}
	}
	return false;
}

void Tower::onPostRender(MinecraftUIRenderContext* renderCtx) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!Game.canUseMoveKeys())
		return;
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto selectedItem = supplies->inventory->getByGlobalIndex(supplies->selectedHotbarSlot);
	if (!selectedItem->isValid || !selectedItem->getItem()->isBlock())  // Block in hand?
		return;

	Vec3 blockBelow = g_Data.getLocalPlayer()->getPos();  // Block below the player
	blockBelow.y -= g_Data.getLocalPlayer()->getAABBShapeComponent()->size.y;
	blockBelow.y -= 0.5f;

	// Adjustment by velocity
	float speed = g_Data.getLocalPlayer()->entityLocation->velocity.magnitudexy();
	Vec3 vel = g_Data.getLocalPlayer()->entityLocation->velocity;
	vel.normalize();  // Only use values from 0 - 1

	if (!tryTower(blockBelow)) {
		if (speed > 0.05f) {
			blockBelow.z -= vel.z * 0.4f;
			if (!tryTower(blockBelow)) {
				blockBelow.x -= vel.x * 0.4f;
				if (!tryTower(blockBelow) && g_Data.getLocalPlayer()->isSprinting()) {
					blockBelow.z += vel.z;
					blockBelow.x += vel.x;
					tryTower(blockBelow);
				}
			}
		}
	}
}
