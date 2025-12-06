#include "RenameCommand.h"
#include "../../../SDK/ItemStack.h"
#include "../../../SDK/Inventory.h"
#include "../../../SDK/InventoryTransaction.h"

RenameCommand::RenameCommand() : IMCCommand("rename", "Renames an item", "<name>") {
}

RenameCommand::~RenameCommand() {
}

bool RenameCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() >= 2);

	PlayerSupplies* supplies = g_Data.getLocalPlayer()->getSupplies();

	auto transactionManager = g_Data.getLocalPlayer()->getTransactionManager();

	PlayerInventory* inv = supplies->inventory;

	int selectedSlot = supplies->selectedHotbarSlot;

	ItemStack* item = inv->getByGlobalIndex(selectedSlot);

	if (item == nullptr || item->item == nullptr) {
		clientMessageF("Please hold an item!");
		return false;
	}

	std::string name;

	for (int i = 1; i < args->size(); i++) {
		name += args->at(i) + " ";
	}

	transactionManager->addInventoryAction(InventoryAction(selectedSlot, item, nullptr, InventorySource(ContainerInventory, inventory, NoFlag)));
	transactionManager->addInventoryAction(InventoryAction(0, nullptr, item, InventorySource(NonImplementedFeatureTODO, Invalid, NoFlag)));

	// TODO: ItemStack::setName() not implemented in SDK
	// item->setName(name);

	transactionManager->addInventoryAction(InventoryAction(0, item, nullptr, InventorySource(NonImplementedFeatureTODO, Invalid, NoFlag)));
	transactionManager->addInventoryAction(InventoryAction(selectedSlot, nullptr, item, InventorySource(ContainerInventory, inventory, NoFlag)));
	transactionManager->addInventoryAction(InventoryAction(selectedSlot, nullptr, item, InventorySource(ContainerInventory, inventory, NoFlag)));

	clientMessageF("Set name to '%s'!", name);
	return true;
}
