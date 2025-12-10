#pragma once

#include "OffsetConstants.h"

// Minecraft 1.21.130 offsets
// Sourced from the current 1.21.130 layout used by Entity/Player wrappers.

struct LocalPlayerOffsets {
    // Sneak is tracked via ActorFlags rather than a direct byte.
    bool useActorFlagSneak = true;
    // EntityLocation* lives at 0x2A0; positions are the first Vec3 inside it.
    int posX         = 0x02A0;
    int posY         = 0x02A4;
    int posZ         = 0x02A8;
    // Player::getHealth fallback used in Player.cpp
    int health       = 0x04B0;
};

struct ItemStackOffsets {
    int count = ITEM_RAW_OFF_COUNT;
    int name  = ITEM_RAW_OFF_NAME;
};

struct PlayerSuppliesOffsets {
    // PlayerSupplies layout: [inventory ptr][selectedHotbarSlot][carriedSlotIndex]
    int selectedSlot = 0x0008;
};
