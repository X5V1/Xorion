#pragma once

#include <cstdint>

//
// PlayerSignatures.h
// Centralized signature names and externs used by Player.cpp and other modules.
// Generated to behave like Horion-style central signature table.
//
// NOTE: If your FindSignature implementation expects a different wildcard syntax,
// adapt the patterns below accordingly.
//

// Offsets validated per build (adjust if you discover different offsets).
// These are the fallbacks Horion-style code uses until signatures are replaced by verified offsets.
constexpr ptrdiff_t OFF_SUPPLIES_PTR_DEFAULT = 0x360; // try 0x360, 0x370, 0x378, 0x3D0 if needed
constexpr ptrdiff_t OFF_DEVICE_ID_DEFAULT   = 0x848;
constexpr ptrdiff_t ITEM_RAW_OFF_PTR        = 0x10;
constexpr ptrdiff_t ITEM_RAW_OFF_COUNT      = 0x14;
constexpr ptrdiff_t ITEM_RAW_OFF_AUX        = 0x18;
constexpr ptrdiff_t ITEM_RAW_OFF_NAME       = 0x20;
constexpr ptrdiff_t ITEM_RAW_OFF_TAG        = 0x40;
constexpr ptrdiff_t LOCALPLAYER_GAMEMODE_OFF= 0xEF8;

// Signature pattern arrays. Ordered high->low reliability.
// These strings are heuristics and should be verified against the target binary.
// If you have a canonical pattern for your build, replace the first string in each list.
namespace Signatures {
    // Player
    extern const char* Player_getName_patterns[];
    extern const char* Player_getHeldItem_patterns[];
    extern const char* Player_getPos_patterns[];
    extern const char* Player_setPos_patterns[];
    extern const char* Player_getHealth_patterns[];
    extern const char* Player_setHealth_patterns[];
    extern const char* Player_sendNetworkPacket_patterns[];

    // LocalPlayer / client-side
    extern const char* LocalPlayer_swing_patterns[];
    extern const char* LocalPlayer_turn_patterns[];
    extern const char* LocalPlayer_applyTurnDelta_patterns[];
    extern const char* LocalPlayer_getGameMode_patterns[];
    extern const char* LocalPlayer_unlockAchievements_patterns[];

    // Preferred fallback vfunc indices (Horion heuristics)
    constexpr int VF_Player_swing_fallback = 56;
    constexpr int VF_Player_sendPacket_fallback = 200;
    constexpr int VF_Player_getHeldItem_fallback = 84;
    constexpr int VF_LocalPlayer_unlockAchievements_fallback = 220;
}
