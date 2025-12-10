#include "BedStack.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/GameMode.h"
#include "../../../SDK/ItemStack.h"
#include "../../../SDK/Inventory.h"

// Inventory size constants
constexpr int TOTAL_INVENTORY_SLOTS = 36; // Hotbar (0-8) + Main inventory (9-35)

// Cache for bed item detection to avoid string operations every tick
// Note: Bed item IDs are typically in the 355-370 range in Bedrock Edition 1.20+
// This covers all 16 bed colors. The range may vary in older versions or with mods.
static inline bool isBedItem(Item* item) {
	if (!item) return false;
	
	// Check by item ID first for efficiency (beds are typically 355-370)
	int itemId = item->itemId;
	if (itemId >= 355 && itemId <= 370) {
		return true;
	}
	
	// Fallback: Check if the name contains "bed" (substring match)
	// This handles mod beds or non-standard IDs, but only called if ID check fails
	const std::string& name = item->name.getText();
	return name.find("bed") != std::string::npos;
}

BedStack::BedStack() : IModule(0, Category::PLAYER, "Allows same-color beds to stack up to 64.") {
	registerIntSetting("MaxStackSize", &maxStackSize, maxStackSize, 1, 64);
}

BedStack::~BedStack() {
}

const char* BedStack::getModuleName() {
	return "BedStack";
}

void BedStack::onTick(C_GameMode* gm) {
	if (!g_Data.getLocalPlayer())
		return;

	auto player = g_Data.getLocalPlayer();
	auto supplies = player->getSupplies();
	if (!supplies || !supplies->inventory)
		return;

	auto inv = supplies->inventory;
	
	// Iterate through all inventory slots
	for (int i = 0; i < TOTAL_INVENTORY_SLOTS; i++) {
		ItemStack* stack = inv->getByGlobalIndex(i);
		if (!stack || !stack->item)
			continue;

		// Check if this is a bed item
		if (isBedItem(stack->item)) {
			// Only modify if properties are not already set to avoid unnecessary operations
			if (stack->item->maxStackSize != static_cast<int16_t>(maxStackSize)) {
				stack->item->setMaxStackSize(static_cast<uint8_t>(maxStackSize));
			}
			// Note: setStackedByData is typically set once and doesn't need repeated calls
			// but we call it to ensure it's properly set for newly acquired beds
			stack->item->setStackedByData(true);
		}
	}
}
