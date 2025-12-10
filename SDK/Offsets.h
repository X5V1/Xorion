#pragma once

// Minecraft 1.21.130 offsets
// Replace placeholder values with real offsets from IDA/Ghidra/x64dbg

struct LocalPlayerOffsets {
    int sneakingFlag = 0x012C; // placeholder
    int posX         = 0x00F0; // placeholder
    int posY         = 0x00F4; // placeholder
    int posZ         = 0x00F8; // placeholder
    int health       = 0x0140; // placeholder
};

struct ItemStackOffsets {
    int count = 0x0010; // placeholder
    int name  = 0x0020; // placeholder
};

struct PlayerSuppliesOffsets {
    int selectedSlot = 0x0008; // placeholder
};
