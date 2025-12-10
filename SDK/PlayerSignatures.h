#pragma once

#include <cstdint>
#include "OffsetConstants.h"

//
// PlayerSignatures.h
// Centralized signature names and externs used by Player.cpp and other modules.
// Generated to behave like Xorion-style central signature table.
//
// NOTE: If your FindSignature implementation expects a different wildcard syntax,
// adapt the patterns below accordingly.
//

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

    // Preferred fallback vfunc indices (Xorion heuristics)
    constexpr int VF_Player_swing_fallback = 56;
    constexpr int VF_Player_sendPacket_fallback = 200;
    constexpr int VF_Player_getHeldItem_fallback = 84;
    constexpr int VF_LocalPlayer_unlockAchievements_fallback = 220;
}
