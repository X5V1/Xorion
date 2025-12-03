#include "AutoClicker.h"

AutoClicker::AutoClicker() : IModule(0, Category::COMBAT, "A simple autoclicker, automatically clicks for you.") {
	registerBoolSetting("RightClick", &rightclick, rightclick);
	registerBoolSetting("Only Weapons", &weapons, weapons);
	registerBoolSetting("Break Blocks", &breakBlocks, breakBlocks);
	registerIntSetting("Delay", &delay, delay, 0, 20);
	registerBoolSetting("Hold", &hold, hold);
}

AutoClicker::~AutoClicker() {}

const char* AutoClicker::getModuleName() {
	return "AutoClicker";
}

void AutoClicker::onTick(GameMode* gm) {
	if ((GameData::isLeftClickDown() || !hold) && GameData::canUseMoveKeys()) {
		LocalPlayer* player = Game.getLocalPlayer();
		Level* level = player->level;
		Odelay++;

		if (Odelay >= delay) {
			auto selectedItem = player->getSelectedItem();
			if (weapons && selectedItem->getAttackingDamageWithEnchants() < 1)
				return;

			player->swingArm();

			if (level->hasEntity() != 0)
				gm->attack(level->getEntity());
			else if (breakBlocks) {
				bool isDestroyed = false;
				gm->startDestroyBlock(level->hitResult.blockPos, (uint8_t)level->hitResult.facing, isDestroyed);
				gm->stopDestroyBlock(level->hitResult.blockPos);
				if (isDestroyed && player->getRegion()->getBlock(level->hitResult.blockPos)->blockLegacy->blockId != 0)
					gm->destroyBlock(&level->hitResult.blockPos, 0);
			}
			Odelay = 0;
		}
	}

	if (rightclick) {
		if ((GameData::isRightClickDown() || !hold) && GameData::canUseMoveKeys()) {
			Level* level = Game.getLocalPlayer()->level;
			Odelay++;
			if (Odelay >= delay) {
				bool idk = true;
				gm->buildBlock(&(Vec3i(level->hitResult.blockPos)), (uint8_t)level->hitResult.facing, true);
				Odelay = 0;
			}
		}
	}
}