#include "OffhandAllow.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/LocalPlayer.h"
#include "../../../SDK/PlayerSupplies.h"

OffhandAllow::OffhandAllow() : IModule(0, Category::MISC, "Let's you place any item in your offhand") {}

OffhandAllow::~OffhandAllow() {}

const char* OffhandAllow::getModuleName() {
	return "OffhandAllow";
}

void OffhandAllow::onEnable() {
	LocalPlayer* player = Game.getLocalPlayer();
	if (player == nullptr) return;

	PlayerSupplies* supplies = player->getSupplies();
	if (supplies == nullptr || supplies->inventory == nullptr) return;

	PlayerInventory* inv = supplies->inventory;
	for (int i = 0; i < 36; i++) {
		ItemStack* itemStack = inv->getByGlobalIndex(i);
		if (itemStack == nullptr || itemStack->item == nullptr)
			continue;

		Item* item = itemStack->getItem();
		if (item != nullptr) {
			item->setAllowOffhand();
		}
	}
}