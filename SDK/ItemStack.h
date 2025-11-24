#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include "../SDK/TextHolder.h"
#include "../SDK/Item.h"
#include "../Utils/Utils.h"

// A relatively full ItemStack representation similar to Horion's SDK wrappers.
// Not all internals are available at compile time; many functions are thin calls
// into the game's memory via resolved vfuncs or signatures. Provide a rich API
// modules expect: comparisons, serialization helpers, enchantments, NBT stubs,
// convenience getters and setters.

struct Enchantment {
    int id = 0;
    int level = 0;
};

struct ItemAttributes {
    int damage = 0;
    int maxDamage = 0;
    int durability = 0;
    // Other attributes can go here (attack, defense, etc.)
};

struct ItemStack {
public:
    // Underlying game pointers (if available)
    void* raw = nullptr;          // pointer to the in-game ItemStack object
    Item* item = nullptr;         // pointer to Item definition
    int count = 0;                // stack count
    int auxValue = 0;             // durability / aux
    int itemId = 0;               // numerical id (cached)
    bool isValid = false;         // indicates proper initialization

    // Optional metadata fields that Horion modules sometimes inspect
    std::vector<Enchantment> enchantments;
    ItemAttributes attributes;
    TextHolder customName;
    std::string customNameStr;    // convenience string copy
    bool isStackTagged = false;   // whether stack has NBT / tags
    // Placeholder for raw NBT / tag object pointer if needed
    void* rawTag = nullptr;

public:
    ItemStack() = default;

    // ctor from raw pointer (game object) — resolves fields lazily
    explicit ItemStack(void* rawPtr) {
        initFromRaw(rawPtr);
    }

    // ctor convenience
    ItemStack(Item* it, int cnt = 1, int aux = 0) {
        item = it;
        count = cnt;
        auxValue = aux;
        itemId = it ? it->itemId : 0;
        isValid = (it != nullptr && cnt > 0);
    }

    // Initialize from a raw game pointer by reading expected offsets.
    // Replace the signatures/offsets with your project's values.
    void initFromRaw(void* rawPtr) {
        raw = rawPtr;
        if (!raw) {
            item = nullptr;
            count = 0;
            auxValue = 0;
            itemId = 0;
            isValid = false;
            return;
        }

        // Example: read common fields at known offsets (adjust per version)
        // These reads are intentionally conservative — verify offsets for your build.
        item = *reinterpret_cast<Item**>(reinterpret_cast<uintptr_t>(raw) + 0x10);
        count = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(raw) + 0x14);
        auxValue = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(raw) + 0x18);
        itemId = item ? item->itemId : 0;
        isValid = (item != nullptr && count > 0);

        // If the game exposes name/tag fields, copy them safely.
        TextHolder* th = reinterpret_cast<TextHolder*>(reinterpret_cast<uintptr_t>(raw) + 0x20);
        if (th) {
            customName = *th;
            customNameStr = customName.getText();
        }
        // Raw tag pointer
        rawTag = *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(raw) + 0x40);
        isStackTagged = (rawTag != nullptr);
        // Attempt to parse enchantments from tag if present (placeholder)
        parseEnchantmentsFromTag();
    }

    // Basic queries
    bool isNull() const { return !isValid || item == nullptr || count <= 0; }
    Item* getItem() const { return item; }
    int getCount() const { return count; }
    void setCount(int c) { count = c; isValid = (item != nullptr && count > 0); }
    int getAuxValue() const { return auxValue; }
    void setAuxValue(int a) { auxValue = a; }

    // Comparison helpers
    bool equals(const ItemStack& other, bool ignoreCount = false) const {
        if (isNull() || other.isNull()) return false;
        if (itemId != other.itemId) return false;
        if (!ignoreCount && count != other.count) return false;
        if (auxValue != other.auxValue) return false;
        // If both have tags, compare tags pointer (shallow). Deep compare optional.
        if (isStackTagged != other.isStackTagged) return false;
        if (isStackTagged && rawTag != other.rawTag) return false;
        return true;
    }

    // Enchantment helpers
    void addEnchantment(int enchId, int level) {
        for (auto &e : enchantments) {
            if (e.id == enchId) {
                e.level = level;
                return;
            }
        }
        enchantments.push_back({enchId, level});
    }

    std::optional<int> getEnchantmentLevel(int enchId) const {
        for (const auto &e : enchantments) if (e.id == enchId) return e.level;
        return std::nullopt;
    }

    // Custom name helpers
    void setCustomName(const std::string& name) {
        customNameStr = name;
        customName.setText(name);
        isStackTagged = true;
        // push into raw NBT if available (optional)
        if (raw) writeCustomNameToRaw();
    }
    std::string getCustomName() const { return customNameStr.empty() ? customName.getText() : customNameStr; }

    // Serialization helpers (stubs). Implement using real NBT serialization as needed.
    std::string serializeToString() const {
        // human-readable fallback serialization for debugging
        if (isNull()) return "<empty>";
        std::string out = item ? item->name.getText() : std::to_string(itemId);
        out += " x" + std::to_string(count);
        if (auxValue != 0) out += " aux:" + std::to_string(auxValue);
        if (!customNameStr.empty()) out += " name:" + customNameStr;
        if (!enchantments.empty()) {
            out += " ench:[";
            for (size_t i = 0; i < enchantments.size(); ++i) {
                out += std::to_string(enchantments[i].id) + ":" + std::to_string(enchantments[i].level);
                if (i + 1 < enchantments.size()) out += ",";
            }
            out += "]";
        }
        return out;
    }

    // High-level operations modules use
    bool consume(int amount = 1) {
        if (isNull() || amount <= 0) return false;
        if (count < amount) return false;
        count -= amount;
        if (count == 0) {
            item = nullptr;
            isValid = false;
        }
        // write to raw if necessary
        if (raw) *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(raw) + 0x14) = count;
        return true;
    }

    void repair(int amount) {
        // if durability/aux tracked as auxValue where 0 is undamaged, reduce aux
        auxValue = std::max(0, auxValue - amount);
        if (raw) *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(raw) + 0x18) = auxValue;
    }

    // Produce a deep copy (not sharing raw pointer)
    ItemStack clone() const {
        ItemStack s;
        s.raw = nullptr;
        s.item = item;
        s.count = count;
        s.auxValue = auxValue;
        s.itemId = itemId;
        s.isValid = isValid;
        s.enchantments = enchantments;
        s.attributes = attributes;
        s.customName = customName;
        s.customNameStr = customNameStr;
        s.isStackTagged = isStackTagged;
        s.rawTag = nullptr;
        return s;
    }

private:
    void parseEnchantmentsFromTag() {
        // Placeholder: inspect rawTag and populate enchantments if present.
        (void)rawTag;
        // Implementation depends on your NBT wrapper.
    }

    void writeCustomNameToRaw() {
        if (!raw) return;
        // Example: store pointer / TextHolder copy back into raw stack at known offset
        TextHolder* dest = reinterpret_cast<TextHolder*>(reinterpret_cast<uintptr_t>(raw) + 0x20);
        if (dest) {
            *dest = customName;
        }
    }
};
