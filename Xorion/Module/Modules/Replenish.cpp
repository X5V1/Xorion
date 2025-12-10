#include "Replenish.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/GameMode.h"
#include "../../../SDK/ItemStack.h"
#include "../../../SDK/Inventory.h"

// Inventory size constants
constexpr int TOTAL_INVENTORY_SLOTS = 36; // Hotbar (0-8) + Main inventory (9-35)

Replenish::Replenish() : IModule(0, Category::COMBAT, "Automatically replenishes items in your hotbar from inventory when they run out.") {
}

Replenish::~Replenish() {
}

const char* Replenish::getModuleName() {
	return "Replenish";
}

void Replenish::onEnable() {
	lastSlot = -1;
	lastItemId = -1;
	lastAuxValue = -1;
}

void Replenish::onDisable() {
	lastSlot = -1;
	lastItemId = -1;
	lastAuxValue = -1;
}

void Replenish::onTick(C_GameMode* gm) {
	if (!g_Data.getLocalPlayer())
		return;

	auto player = g_Data.getLocalPlayer();
	auto supplies = player->getSupplies();
	if (!supplies || !supplies->inventory)
		return;

	auto inv = supplies->inventory;
	int currentSlot = supplies->selectedHotbarSlot;

	// Get the current item in the selected hotbar slot
	ItemStack* selectedStack = inv->getByGlobalIndex(currentSlot);
	
	// Check if we need to track this slot
	if (selectedStack && selectedStack->item) {
		// If this is a new slot or different item, track it
		if (currentSlot != lastSlot || selectedStack->item->itemId != lastItemId) {
			lastSlot = currentSlot;
			lastItemId = selectedStack->item->itemId;
			lastAuxValue = selectedStack->auxValue;
		}
	}

	// Check if the slot is now empty but we had something there before
	if (lastSlot == currentSlot && lastItemId != -1) {
		if (!selectedStack || !selectedStack->item || selectedStack->count == 0) {
			// The slot is now empty, search for matching items in inventory
			bool foundReplacement = false;
			
			// Search through all inventory slots
			for (int i = 0; i < TOTAL_INVENTORY_SLOTS; i++) {
				// Skip the current slot
				if (i == currentSlot)
					continue;

				ItemStack* stack = inv->getByGlobalIndex(i);
				if (!stack || !stack->item || stack->count == 0)
					continue;

				// Check if this item matches what we had
				// For beds, we match by item type (bed) and aux value (color)
				// For other items, we match by itemId and auxValue
				bool matches = false;
				if (stack->item->itemId == lastItemId) {
					// For most items, also match aux value
					if (lastAuxValue == stack->auxValue) {
						matches = true;
					}
				}

				if (matches) {
					// Move this stack to the empty hotbar slot
					// Note: Direct assignment is safe here as ItemStack supports copy assignment
					// This pattern is used throughout the codebase (see AutoArmor.cpp)
					*selectedStack = *stack;
					
					// Clear the source slot
					stack->item = nullptr;
					stack->count = 0;
					stack->auxValue = 0;
					
					foundReplacement = true;
					break;
				}
			}

			// If we found a replacement, update tracking
			if (foundReplacement) {
				// Keep the same lastItemId and lastAuxValue for potential future replenishments
			} else {
				// No replacement found, reset tracking
				lastSlot = -1;
				lastItemId = -1;
				lastAuxValue = -1;
			}
		}
	}
}
