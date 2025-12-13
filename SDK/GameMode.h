// SDK/GameMode.h
#pragma once

#include <cstdint>
#include "Vec3.h"
#include "Vec3i.h"
#include "ItemStack.h"

// Forward declaration
struct Entity;

// Unified GameMode struct
struct GameMode {
    // Members
    Entity* player = nullptr; // Pointer to the player entity
    
    // Commonly used calls; adjust to your actual signatures and implementations.
    void buildBlock(Vec3i* blockPos, uint8_t face, bool useBlockSide);
    void useItemOn(ItemStack* stack, const Vec3i& blockPos, uint8_t face, const Vec3& hitPos);
    void attack(Entity* target);
    void startDestroyBlock(const Vec3i& pos, uint8_t face) { /* stub */ }
    void stopDestroyBlock() { /* stub */ }
    void destroyBlock(const Vec3i& pos, uint8_t face) { /* stub */ }
    void baseUseItem(ItemStack* stack) { /* stub */ }
};

// Alias for legacy code
using C_GameMode = GameMode;

// Function pointer types (fill in real signatures as you confirm them)
using GameMode_AttackFn = void(*)(GameMode*, Entity*);
using GameMode_SomeOtherFn = int64_t(*)(GameMode*, int);

// Exposed typed pointers
namespace SDK {
    extern GameMode_AttackFn GameMode_attack;
    extern GameMode_SomeOtherFn GameMode_someOther;

    inline bool GameMode_attack_available() { return GameMode_attack != nullptr; }
    inline bool GameMode_someOther_available() { return GameMode_someOther != nullptr; }
}

// Init functions (call after signatures are resolved)
void Init_GameMode_Attack();
void Init_GameMode_Other();
void Init_AllGameModeMappings(); // convenience to call both
