#pragma once

#include <array>
#include <vector>
#include <optional>
#include <cstdint>
#include "ItemStack.h"
#include "../SDK/TextHolder.h"
#include "../Utils/Utils.h"

// PlayerSupplies is a Xorion-style wrapper for a player's inventory/hotbar/armor/offhand.
// Provide a complete set of helpers modules call: searching, swapping, adding, removing,
// quick-moving, serialization, slot indexes, and network updates.

constexpr int HOTBAR_SIZE = 9;
constexpr int ARMOR_SIZE = 4;
constexpr int MAIN_INV_ROWS = 3;
constexpr int MAIN_INV_COLS = 9;
constexpr int MAIN_INV_SIZE = MAIN_INV_ROWS * MAIN_INV_COLS;
constexpr int FULL_INV_SIZE = HOTBAR_SIZE + MAIN_INV_SIZE + ARMOR_SIZE + 1; // + offhand

struct ContainerSlot {
    ItemStack stack;
    int slotIndex = -1;
};

struct Inventory {
    // Layout assumption: [hotbar(0..8)] [main inventory(9..35)] [armor slots 36..39] [offhand 40]
    std::array<ItemStack, HOTBAR_SIZE> hotbar;
    std::array<ItemStack, MAIN_INV_SIZE> mainInv;
    std::array<ItemStack, ARMOR_SIZE> armor;
    ItemStack offhand;

    // getters
    ItemStack* getHotbarSlot(int idx) {
        if (idx < 0 || idx >= HOTBAR_SIZE) return nullptr;
        return &hotbar[idx];
    }
    ItemStack* getMainSlot(int idx) {
        if (idx < 0 || idx >= MAIN_INV_SIZE) return nullptr;
        return &mainInv[idx];
    }
    ItemStack* getArmorSlot(int idx) {
        if (idx < 0 || idx >= ARMOR_SIZE) return nullptr;
        return &armor[idx];
    }
    ItemStack* getOffhandSlot() { return &offhand; }

    // find first matching slot index in whole inventory, -1 if none
    int findFirstMatching(Item* itemToFind, bool ignoreCount = true) {
        if (!itemToFind) return -1;
        for (int i = 0; i < HOTBAR_SIZE; ++i) {
            if (hotbar[i].getItem() == itemToFind) return i;
        }
        for (int i = 0; i < MAIN_INV_SIZE; ++i) {
            if (mainInv[i].getItem() == itemToFind) return HOTBAR_SIZE + i;
        }
        for (int i = 0; i < ARMOR_SIZE; ++i) {
            if (armor[i].getItem() == itemToFind) return HOTBAR_SIZE + MAIN_INV_SIZE + i;
        }
        if (offhand.getItem() == itemToFind) return HOTBAR_SIZE + MAIN_INV_SIZE + ARMOR_SIZE;
        return -1;
    }

    // get pointer to slot by global index scheme used by many modules
    ItemStack* getByGlobalIndex(int globalIdx) {
        if (globalIdx >= 0 && globalIdx < HOTBAR_SIZE) return &hotbar[globalIdx];
        int mainStart = HOTBAR_SIZE;
        if (globalIdx >= mainStart && globalIdx < mainStart + MAIN_INV_SIZE) return &mainInv[globalIdx - mainStart];
        int armorStart = mainStart + MAIN_INV_SIZE;
        if (globalIdx >= armorStart && globalIdx < armorStart + ARMOR_SIZE) return &armor[globalIdx - armorStart];
        int offhandIdx = armorStart + ARMOR_SIZE;
        if (globalIdx == offhandIdx) return &offhand;
        return nullptr;
    }
};

struct PlayerSupplies {
    Inventory* inventory = nullptr;   // raw pointer to game's Inventory structure
    int selectedHotbarSlot = 0;       // 0..8
    int carriedSlotIndex = -1;        // slot being carried (dragging) for GUIs

    // Convenience wrappers used everywhere in modules
    ItemStack* getItemStack(int globalSlotIndex) {
        if (!inventory) return nullptr;
        return inventory->getByGlobalIndex(globalSlotIndex);
    }

    ItemStack* getHotbarSelectedItem() {
        if (!inventory) return nullptr;
        return inventory->getHotbarSlot(selectedHotbarSlot);
    }

    void setSelectedHotbarSlot(int idx) {
        if (idx < 0 || idx >= HOTBAR_SIZE) return;
        selectedHotbarSlot = idx;
        // Optionally, send a packet or vfunc to update server/client selected slot
        // e.g., Utils::CallVFunc</*idx*/ 150, void>(ownerPlayer, idx);
    }

    // Add an item stack to inventory — tries hotbar first, then main inv, returns true if fully added
    bool addItemStack(const ItemStack& stack) {
        if (!inventory || stack.isNull()) return false;

        // Try to merge into existing stacks of same item (hotbar then main)
        for (int i = 0; i < HOTBAR_SIZE; ++i) {
            auto &s = inventory->hotbar[i];
            if (!s.isNull() && s.getItem() == stack.getItem() && s.auxValue == stack.auxValue) {
                // naive merge ignoring stack limits for now
                s.setCount(s.getCount() + stack.getCount());
                return true;
            }
        }
        for (int i = 0; i < MAIN_INV_SIZE; ++i) {
            auto &s = inventory->mainInv[i];
            if (!s.isNull() && s.getItem() == stack.getItem() && s.auxValue == stack.auxValue) {
                s.setCount(s.getCount() + stack.getCount());
                return true;
            }
        }

        // Place in first empty hotbar slot
        for (int i = 0; i < HOTBAR_SIZE; ++i) {
            auto &s = inventory->hotbar[i];
            if (s.isNull()) {
                s = stack.clone();
                return true;
            }
        }
        // Place in first empty main slot
        for (int i = 0; i < MAIN_INV_SIZE; ++i) {
            auto &s = inventory->mainInv[i];
            if (s.isNull()) {
                s = stack.clone();
                return true;
            }
        }
        // No space
        return false;
    }

    // Remove count from a specified global slot, returns true if removal successful
    bool removeFromSlot(int globalIdx, int countToRemove) {
        ItemStack* s = getItemStack(globalIdx);
        if (!s || s->isNull()) return false;
        if (s->getCount() < countToRemove) return false;
        s->setCount(s->getCount() - countToRemove);
        if (s->getCount() <= 0) {
            *s = ItemStack(); // reset slot
        }
        return true;
    }

    // Swap two global slots (for quickmove / GUI)
    bool swapSlots(int aGlobal, int bGlobal) {
        ItemStack* a = getItemStack(aGlobal);
        ItemStack* b = getItemStack(bGlobal);
        if (!a || !b) return false;
        std::swap(*a, *b);
        return true;
    }

    // Find first slot that satisfies predicate
    template<typename Predicate>
    int findSlotIf(Predicate p) const {
        if (!inventory) return -1;
        for (int i = 0; i < HOTBAR_SIZE; ++i) {
            if (p(inventory->hotbar[i])) return i;
        }
        for (int i = 0; i < MAIN_INV_SIZE; ++i) {
            if (p(inventory->mainInv[i])) return HOTBAR_SIZE + i;
        }
        for (int i = 0; i < ARMOR_SIZE; ++i) {
            if (p(inventory->armor[i])) return HOTBAR_SIZE + MAIN_INV_SIZE + i;
        }
        if (p(inventory->offhand)) return HOTBAR_SIZE + MAIN_INV_SIZE + ARMOR_SIZE;
        return -1;
    }

    // Quick convenience: find an item by Item* pointer
    int findItem(Item* it) const {
        if (!inventory || !it) return -1;
        return findSlotIf([it](const ItemStack& s) { return !s.isNull() && s.getItem() == it; });
    }

    // Drop an item from a global slot to the world (stub — actual send to server or spawn)
    void dropItemFromSlot(int globalIdx, bool wholeStack = true) {
        ItemStack* s = getItemStack(globalIdx);
        if (!s || s->isNull()) return;
        if (wholeStack) {
            // spawn in world or call engine API. Here we just clear the slot.
            *s = ItemStack();
        } else {
            // remove single
            removeFromSlot(globalIdx, 1);
        }
        // Hook network update if necessary
        onInventoryChanged(globalIdx);
    }

    // Notify engine that slot changed; modules rely on this to refresh UI or send packets
    void onInventoryChanged(int slotIdx) {
        // Placeholder: in Xorion this often calls Player::sendInventory or a vfunc on Player
        (void)slotIdx;
        // e.g., Utils::CallVFunc</*idx*/ 200, void>(ownerPlayer, ...);
    }

    // Serializes inventory to a debug string (useful in logs)
    std::string serializeInventoryDebug() const {
        std::string out;
        if (!inventory) return "<no-inventory>";
        out += "Hotbar: ";
        for (int i = 0; i < HOTBAR_SIZE; ++i) {
            out += inventory->hotbar[i].serializeToString();
            if (i + 1 < HOTBAR_SIZE) out += ", ";
        }
        out += "\nMain: ";
        for (int i = 0; i < MAIN_INV_SIZE; ++i) {
            out += inventory->mainInv[i].serializeToString();
            if (i + 1 < MAIN_INV_SIZE) out += ", ";
        }
        out += "\nArmor: ";
        for (int i = 0; i < ARMOR_SIZE; ++i) {
            out += inventory->armor[i].serializeToString();
            if (i + 1 < ARMOR_SIZE) out += ", ";
        }
        out += "\nOffhand: ";
        out += inventory->offhand.serializeToString();
        return out;
    }
};
