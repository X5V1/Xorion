// SDK/GameMode.h
#pragma once

#include <cstdint>
#include "Entity.h"
#include "Vec3.h"
#include "Vec3i.h"
#include "ItemStack.h"

// Unified GameMode struct
struct GameMode {
    // Commonly used calls; adjust to your actual signatures and implementations.
    void buildBlock(Vec3i* blockPos, uint8_t face, bool useBlockSide);
    void useItemOn(ItemStack* stack, const Vec3i& blockPos, uint8_t face, const Vec3& hitPos);
};

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
