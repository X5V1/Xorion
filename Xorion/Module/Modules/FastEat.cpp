#include "FastEat.h"
#include "../../../Memory/GameData.h"

FastEat::FastEat() : IModule(0, Category::PLAYER, "Eat food almost instantly.") {
}

FastEat::~FastEat() {
}

const char* FastEat::getModuleName() {
	return ("FastEat");
}

void FastEat::onTick(C_GameMode* gm) {
	PlayerSupplies* supplies = g_Data.getLocalPlayer()->getSupplies();
	PlayerInventory* inv = supplies->inventory;
	for (int i = 0; i < 36; i++) {
		ItemStack* stack = inv->getByGlobalIndex(i);
		if (stack->item != NULL && stack->item->itemId != 261 && stack->item->duration == 32) {
			stack->item->setMaxUseDuration(5);
		}
	}
}

void FastEat::onDisable() {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	PlayerSupplies* supplies = g_Data.getLocalPlayer()->getSupplies();
	PlayerInventory* inv = supplies->inventory;
	for (int i = 0; i < 36; i++) {
		ItemStack* stack = inv->getByGlobalIndex(i);
		if (stack->item != NULL && stack->item->itemId != 261 && stack->item->duration == 5) {
			stack->item->setMaxUseDuration(32);
		}
	}
}
