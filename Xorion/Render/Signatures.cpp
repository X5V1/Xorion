#include "PlayerSignatures.h"
#include "../Utils/Utils.h"
#include "../SDK/FindSignatures.h"
#include <vector>
#include <cstdio>
#include <cstring>

// Implementation of pattern arrays
namespace Signatures {

    // Player_getName
    const char* Player_getName_patterns[] = {
        // High reliability candidates
        "48 89 5C 24 ? 57 48 83 EC ? 48 8B F9 48 8B 49 ? 48 8B 01 FF 50 ?",
        // Medium
        "48 89 5C 24 ? 57 48 83 EC ? 48 8B F9 E8 ? ? ? ? 48 8B C8",
        // Low
        "48 8B 05 ? ? ? ? 48 8B 40 ? 48 8B 88 ? ? ? ? 48 85 C9",
        nullptr
    };

    // Player_getHeldItem
    const char* Player_getHeldItem_patterns[] = {
        "48 8B C4 55 57 41 54 41 55 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ?",
        "48 8B 01 48 8B 40 10 48 89 44 24 ? 48 8B 40 18",
        nullptr
    };

    // Player_getPos
    const char* Player_getPos_patterns[] = {
        "48 89 5C 24 ? 57 48 83 EC ? 48 8B F9 48 8B 49 ? 48 8B 01 48 8B 40 ?",
        "48 8B 05 ? ? ? ? 48 8B 50 ? 48 8B 88 ? ? ? ? 48 85 C9",
        nullptr
    };

    // Player_setPos
    const char* Player_setPos_patterns[] = {
        "48 89 5C 24 ? 57 48 83 EC ? 48 8B F9 48 8B 49 ? 48 89 81 ? ? ? ?",
        "48 89 5C 24 ? 55 48 8B EC 48 83 EC ? 48 89 4D ? F3 0F 11 41 ?",
        nullptr
    };

    // Player_getHealth
    const char* Player_getHealth_patterns[] = {
        "F3 0F 10 41 ? 48 8B C8 E8 ? ? ? ? F3 0F 10 05 ? ? ? ?",
        "48 89 5C 24 ? 55 48 8B EC 48 83 EC ? 48 89 4D ? F3 0F 11 41 ?",
        nullptr
    };

    // Player_setHealth
    const char* Player_setHealth_patterns[] = {
        "48 89 5C 24 ? 55 48 8B EC 48 83 EC ? 48 89 4D ? F3 0F 11 41 ?",
        nullptr
    };

    // Player_sendNetworkPacket
    const char* Player_sendNetworkPacket_patterns[] = {
        "48 8B C4 57 48 83 EC ? 48 8B 7D ? 48 8B 05 ? ? ? ? 48 8B 00 FF 50 ?",
        nullptr
    };

    // LocalPlayer_swing
    const char* LocalPlayer_swing_patterns[] = {
        "40 53 48 83 EC ? 48 8B D9 48 8B 0D ? ? ? ? 48 8B 01 FF 50 ?",
        "48 89 5C 24 ? 48 89 7C 24 ? 57 48 83 EC ? 8B F9 E8 ? ? ? ?",
        nullptr
    };

    // LocalPlayer_turn
    const char* LocalPlayer_turn_patterns[] = {
        "48 8B 58 68 48 83 EC ? 48 8B F1 48 8B 4A ? E8 ? ? ? ?",
        "48 8B 05 ? ? ? ? 48 8B 80 ? ? ? ? 48 8B D1 E8 ? ? ? ?",
        nullptr
    };

    // LocalPlayer_applyTurnDelta
    const char* LocalPlayer_applyTurnDelta_patterns[] = {
        "48 89 5C 24 ? 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 8B F9 48 8B 01 48 8B 68 ?",
        nullptr
    };

    // LocalPlayer_getGameMode
    const char* LocalPlayer_getGameMode_patterns[] = {
        "48 8B 81 ? ? ? ? 48 8B 48 ? 48 8B 01 FF 50 ?",
        "48 8B 05 ? ? ? ? 48 8B 50 ? 48 8B 88 ? ? ? ? 48 85 C9",
        nullptr
    };

    // LocalPlayer_unlockAchievements
    const char* LocalPlayer_unlockAchievements_patterns[] = {
        "40 53 48 83 EC ? 48 8B D9 48 8B 49 ? E8 ? ? ? ? 48 8B C8",
        nullptr
    };
}

// Helper that logs resolution (Xorion-style)
static void logResolution(const char* name, const char* pattern, void* addr) {
#if defined(_MSC_VER)
    // Windows: use OutputDebugString if desired via Utils or fallback to printf
    if (addr) {
        Utils::Log("[Signatures] Resolved %s -> %p (pattern: %s)\n", name, addr, pattern);
    } else {
        Utils::Warn("[Signatures] Failed to resolve %s (last tried pattern: %s)\n", name, pattern);
    }
#else
    if (addr) {
        printf("[Signatures] Resolved %s -> %p (pattern: %s)\n", name, addr, pattern);
    } else {
        printf("[Signatures] Failed to resolve %s (last tried pattern: %s)\n", name, pattern);
    }
#endif
}

// Public resolver: tries a list until first match
void* ResolveFirst(const char** patterns, const char* name) {
    if (!patterns) return nullptr;
    const char* last = nullptr;
    for (size_t i = 0; patterns[i] != nullptr; ++i) {
        last = patterns[i];
        void* r = FindSignature(patterns[i]);
        if (r) {
            logResolution(name, patterns[i], r);
            return r;
        }
    }
    logResolution(name, last ? last : "(none)", nullptr);
    return nullptr;
}

// Convenience wrapper to attempt each array (exposed usage below)
void* ResolvePlayerGetName() { return ResolveFirst(Signatures::Player_getName_patterns, "Player_getName"); }
void* ResolvePlayerGetHeldItem() { return ResolveFirst(Signatures::Player_getHeldItem_patterns, "Player_getHeldItem"); }
void* ResolvePlayerGetPos() { return ResolveFirst(Signatures::Player_getPos_patterns, "Player_getPos"); }
void* ResolvePlayerSetPos() { return ResolveFirst(Signatures::Player_setPos_patterns, "Player_setPos"); }
void* ResolvePlayerGetHealth() { return ResolveFirst(Signatures::Player_getHealth_patterns, "Player_getHealth"); }
void* ResolvePlayerSetHealth() { return ResolveFirst(Signatures::Player_setHealth_patterns, "Player_setHealth"); }
void* ResolvePlayerSendPacket() { return ResolveFirst(Signatures::Player_sendNetworkPacket_patterns, "Player_sendNetworkPacket"); }

void* ResolveLocalPlayerSwing() { return ResolveFirst(Signatures::LocalPlayer_swing_patterns, "LocalPlayer_swing"); }
void* ResolveLocalPlayerTurn() { return ResolveFirst(Signatures::LocalPlayer_turn_patterns, "LocalPlayer_turn"); }
void* ResolveLocalPlayerApplyTurn() { return ResolveFirst(Signatures::LocalPlayer_applyTurnDelta_patterns, "LocalPlayer_applyTurnDelta"); }
void* ResolveLocalPlayerGetGameMode() { return ResolveFirst(Signatures::LocalPlayer_getGameMode_patterns, "LocalPlayer_getGameMode"); }
void* ResolveLocalPlayerUnlock() { return ResolveFirst(Signatures::LocalPlayer_unlockAchievements_patterns, "LocalPlayer_unlockAchievements"); }
