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
	if ((g_Data.isLeftClickDown() || !hold) && g_Data.canUseMoveKeys()) {
		LocalPlayer* player = Game.getLocalPlayer();
		Level* level = player->level;
		Odelay++;

		if (Odelay >= delay) {
			auto supplies = player->getSupplies();
			auto selectedItem = supplies->inventory->getByGlobalIndex(supplies->selectedHotbarSlot);
			if (weapons && selectedItem && selectedItem->getAttackingDamageWithEnchants() < 1)
				return;

			player->swingArm();

		if (level->hasEntity() != 0)
			gm->attack(level->getEntity());
		else if (breakBlocks) {
			gm->startDestroyBlock(level->hitResult.blockPos, (uint8_t)level->hitResult.facing);
			gm->stopDestroyBlock();
			if (player->getRegion()->getBlock(level->hitResult.blockPos)->blockLegacy->blockId != 0)
				gm->destroyBlock(level->hitResult.blockPos, 0);
			}
			Odelay = 0;
		}
	}

	if (rightclick) {
		if ((g_Data.isRightClickDown() || !hold) && g_Data.canUseMoveKeys()) {
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
