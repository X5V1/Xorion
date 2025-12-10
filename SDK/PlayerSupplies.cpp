// PlayerSupplies.cpp
#include "PlayerSupplies.h"
#include <stdexcept>
#include "Utils/Logger.h"

int   (*PlayerSupplies::PlayerSupplies_getSelectedSlot_fn)(void*) = nullptr;
void  (*PlayerSupplies::PlayerSupplies_setSelectedSlot_fn)(void*, int) = nullptr;
void* (*PlayerSupplies::PlayerSupplies_getItemStack_fn)(void*, int) = nullptr;

void PlayerSupplies::ensureIntegration() const {
    if (!suppliesPtr ||
        !PlayerSupplies_getSelectedSlot_fn ||
        !PlayerSupplies_getItemStack_fn) {
        logF("[PlayerSupplies] Missing integration. Ensure InitSDK() succeeded for 1.21.124.");
        throw std::runtime_error("PlayerSupplies SDK not initialized");
    }
}

int PlayerSupplies::getSelectedHotbarSlot() const {
    ensureIntegration();
    return PlayerSupplies_getSelectedSlot_fn(suppliesPtr);
}

void PlayerSupplies::setSelectedHotbarSlot(int slot) {
    ensureIntegration();
    if (!PlayerSupplies_setSelectedSlot_fn) {
        logF("[PlayerSupplies] setSelectedHotbarSlot signature missing for 1.21.124");
        return;
    }
    PlayerSupplies_setSelectedSlot_fn(suppliesPtr, slot);
}

ItemStack PlayerSupplies::getSelectedItem() const {
    ensureIntegration();
    int slot = PlayerSupplies_getSelectedSlot_fn(suppliesPtr);
    void* itemPtr = PlayerSupplies_getItemStack_fn(suppliesPtr, slot);
    return ItemStack(itemPtr);
}

ItemStack PlayerSupplies::getItemStackAt(int slot) const {
    ensureIntegration();
    void* itemPtr = PlayerSupplies_getItemStack_fn(suppliesPtr, slot);
    return ItemStack(itemPtr);
}
