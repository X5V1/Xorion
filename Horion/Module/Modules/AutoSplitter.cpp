#include "AutoSplitter.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/LocalPlayer.h"
#include "../../../SDK/PlayerSupplies.h"
#include "../../../SDK/InventoryTransaction.h"
#include "../../../Utils/Logger.h"

AutoSplitter::AutoSplitter() : IModule(0, Category::PLAYER, "Splits blocks in your inventory.") {
	registerIntSetting("Max amount", &this->maxAmount, this->maxAmount, 4, 64);
	registerIntSetting("Delay (seconds)", &this->delay, this->delay, 1, 5);
}

AutoSplitter::~AutoSplitter() { }

const char* AutoSplitter::getModuleName() {
	return "AutoSplitter";
}

void AutoSplitter::onTick(GameMode* gm) {
	LocalPlayer* player = Game.getLocalPlayer();
	if (player == nullptr) return;

	this->ticks++;

	PlayerSupplies* supplies = player->getSupplies();
	if (supplies == nullptr || supplies->inventory == nullptr) return;

	auto manager = player->getTransactionManager();
	if (manager == nullptr) return;

	PlayerInventory* inv = supplies->inventory;

	if (this->ticks % 5 == 0) {
		if (!this->queue.empty()) {
			for (std::vector<int>::iterator it = this->queue.begin(); it != this->queue.end(); it++) {
				// Drop slot functionality - simplified
			}
			this->queue.clear();
		}
	}

	if (this->ticks >= this->delay * 20) {
		int emptySlots = 0;
		for (int i = 0; i < 36; i++) {
			ItemStack* stack = inv->getByGlobalIndex(i);
			if (stack == nullptr || stack->getItem() == nullptr || stack->isNull()) {
				emptySlots++;
			}
		}

		if (emptySlots == 0) {
			Game.log("[AutoSplitter] Please empty your inventory!");
			this->setEnabled(false);
			return;
		}

		for (int i = 0; i < 36; i++) {
			ItemStack* stack = inv->getByGlobalIndex(i);

			if (stack == nullptr || stack->item == nullptr) continue;

			if (!stack->getItem()->isBlock()) continue;

			if (stack->count > this->maxAmount) {
				ItemStack tmp1(*stack);
				tmp1.count = this->maxAmount;

				ItemStack tmp2(*stack);
				tmp2.count = stack->count - this->maxAmount;

				int empty = inv->getFirstEmptySlot();
				if (empty < 0) break;

				InventoryAction first(i, stack, nullptr);
				InventoryAction second(empty, nullptr, &tmp2);
				InventoryAction third(i, nullptr, &tmp1);

				manager->addInventoryAction(first);
				manager->addInventoryAction(second);
				manager->addInventoryAction(third);

				this->queue.push_back(empty);
				break;
			}
		}

		this->ticks = 0;
	}
}
