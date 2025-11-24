#pragma once

#include <cstddef>
#include "../Utils/HMath.h"

// Forward declaration to avoid heavy includes
struct Entity;

// Use struct consistently to avoid C4099 warnings.
struct EntityList {
private:
    char pad_0x0000[0x70];  // 0x0000

public:
    uintptr_t firstEntity;  // 0x0050
    uintptr_t lastEntity;   // 0x0058
    uintptr_t endAddr;      // 0x0060

    inline size_t getListSize() const {
        return (lastEntity - firstEntity) / sizeof(uintptr_t);
    }

    inline Entity* get(size_t idx) const {
        if (idx >= getListSize())
            return nullptr;
        return *reinterpret_cast<Entity**>(firstEntity + (idx * sizeof(uintptr_t)));
    }

    inline Entity* operator[](size_t idx) const {
        return get(idx);
    }
};  // Size matches layout expectations

struct MobEntityList {
private:
    char pad_0x0000[0x38];  // 0x0000

public:
    uintptr_t firstEntity;  // 0x0038
    uintptr_t lastEntity;   // 0x0040
    uintptr_t endAddr;      // 0x0048

    inline size_t getListSize() const {
        return (lastEntity - firstEntity) / sizeof(uintptr_t);
    }

    inline Entity* get(size_t idx) const {
        if (idx >= getListSize())
            return nullptr;
        return *reinterpret_cast<Entity**>(firstEntity + (idx * sizeof(uintptr_t)));
    }

    inline Entity* operator[](size_t idx) const {
        return get(idx);
    }
};  // Size matches layout expectations
