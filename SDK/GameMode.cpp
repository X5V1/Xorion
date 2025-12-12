// SDK/GameMode.cpp
#include "SDK/GameMode.h"
#include "Memory/SignatureGlobals.h"
#include "../Utils/Logger.h"
#include <cstring> // for memcmp if doing quick runtime validation

namespace SDK {
    GameMode_AttackFn    GameMode_attack   = nullptr;
    GameMode_SomeOtherFn GameMode_someOther = nullptr;
}

// Small helper to do a quick bytes-based sanity check at the resolved address.
// You can expand or remove this if you prefer manual x64dbg verification.
static bool QuickValidateSignature(void* addr, const unsigned char* expectedPrefix, size_t len) {
    if (!addr || !expectedPrefix || len == 0) return false;
    // Memory at addr must be readable by the process; keep the check light.
    // This is a best-effort guard to avoid obvious mismatches.
    __try {
        if (std::memcmp(addr, expectedPrefix, len) == 0) return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
    return false;
}

// Known good leading bytes for our preferred candidates (use short slices).
// Update these if you change preferred patterns after manual inspection.
static const unsigned char kWrapperEarlyCallPrefix[] = {
    0x48, 0x89, 0x5C, 0x24, 0x08, 0x57, 0x48, 0x83, 0xEC, 0x20
};
static const unsigned char kWrapperCallBPrefix[] = {
    0x48, 0x89, 0x5C, 0x24, 0x08, 0x57, 0x48, 0x83, 0xEC, 0x20
};
static const unsigned char kTwoArgCallAPrefix[] = {
    0x48, 0x89, 0x5C, 0x24, 0x08, 0x57, 0x48, 0x83, 0xEC, 0x20
};

// Try to map GameMode::attack using a prioritized list of candidate globals.
// Each candidate is a pointer to a resolved global (from SignatureGlobals).
// The routine validates each candidate briefly and logs the chosen mapping.
void Init_GameMode_Attack() {
    logF("[GameMode] Init_GameMode_Attack starting.");

    // Preferred candidate: early wrapper
    if (g_WrapperEarlyCall) {
        if (QuickValidateSignature(g_WrapperEarlyCall, kWrapperEarlyCallPrefix, sizeof(kWrapperEarlyCallPrefix))) {
            SDK::GameMode_attack = reinterpret_cast<GameMode_AttackFn>(g_WrapperEarlyCall);
            logF("[GameMode] Mapped GameMode_attack -> g_WrapperEarlyCall (%p)", g_WrapperEarlyCall);
            return;
        } else {
            logF("[GameMode] Candidate g_WrapperEarlyCall (%p) failed quick validation", g_WrapperEarlyCall);
        }
    }

    // Fallback candidate: wrapper call B
    if (g_WrapperCallB) {
        if (QuickValidateSignature(g_WrapperCallB, kWrapperCallBPrefix, sizeof(kWrapperCallBPrefix))) {
            SDK::GameMode_attack = reinterpret_cast<GameMode_AttackFn>(g_WrapperCallB);
            logF("[GameMode] Fallback mapped GameMode_attack -> g_WrapperCallB (%p)", g_WrapperCallB);
            return;
        } else {
            logF("[GameMode] Candidate g_WrapperCallB (%p) failed quick validation", g_WrapperCallB);
        }
    }

    // Secondary fallback: two-arg call candidate
    if (g_TwoArgCallA) {
        if (QuickValidateSignature(g_TwoArgCallA, kTwoArgCallAPrefix, sizeof(kTwoArgCallAPrefix))) {
            SDK::GameMode_attack = reinterpret_cast<GameMode_AttackFn>(g_TwoArgCallA);
            logF("[GameMode] Fallback mapped GameMode_attack -> g_TwoArgCallA (%p)", g_TwoArgCallA);
            return;
        } else {
            logF("[GameMode] Candidate g_TwoArgCallA (%p) failed quick validation", g_TwoArgCallA);
        }
    }

    // Last resort: any non-null candidate from signature globals
    if (g_WrapperEarlyCall) {
        SDK::GameMode_attack = reinterpret_cast<GameMode_AttackFn>(g_WrapperEarlyCall);
        logF("[GameMode] Forced mapping GameMode_attack -> g_WrapperEarlyCall (%p) without prefix match", g_WrapperEarlyCall);
        return;
    }
    if (g_WrapperCallB) {
        SDK::GameMode_attack = reinterpret_cast<GameMode_AttackFn>(g_WrapperCallB);
        logF("[GameMode] Forced mapping GameMode_attack -> g_WrapperCallB (%p) without prefix match", g_WrapperCallB);
        return;
    }
    if (g_TwoArgCallA) {
        SDK::GameMode_attack = reinterpret_cast<GameMode_AttackFn>(g_TwoArgCallA);
        logF("[GameMode] Forced mapping GameMode_attack -> g_TwoArgCallA (%p) without prefix match", g_TwoArgCallA);
        return;
    }

    // Nothing mapped
    SDK::GameMode_attack = nullptr;
    logF("[GameMode] WARNING: GameMode_attack not mapped. Dependent features disabled.");
}

// Example mapping for another GameMode-related function. Use the best matching signature for this one.
void Init_GameMode_Other() {
    logF("[GameMode] Init_GameMode_Other starting.");

    // Suppose PATTERN_MethodRCX_RDX or PATTERN_FieldReadCall corresponds to this function.
    if (g_MethodRCX_RDX) {
        SDK::GameMode_someOther = reinterpret_cast<GameMode_SomeOtherFn>(g_MethodRCX_RDX);
        logF("[GameMode] Mapped GameMode_someOther -> g_MethodRCX_RDX (%p)", g_MethodRCX_RDX);
        return;
    }

    if (g_FieldReadCall) {
        SDK::GameMode_someOther = reinterpret_cast<GameMode_SomeOtherFn>(g_FieldReadCall);
        logF("[GameMode] Fallback mapped GameMode_someOther -> g_FieldReadCall (%p)", g_FieldReadCall);
        return;
    }

    SDK::GameMode_someOther = nullptr;
    logF("[GameMode] WARNING: GameMode_someOther not mapped.");
}

void Init_AllGameModeMappings() {
    Init_GameMode_Attack();
    Init_GameMode_Other();
}

// Stub implementation for useItemOn - needs signature resolution
void GameMode::useItemOn(ItemStack* stack, const Vec3i& blockPos, uint8_t face, const Vec3 & hitPos) {
    // TODO: Resolve through signature scanning in 1.21.130
    // For now, log and skip to prevent linker errors
    logF("[GameMode] useItemOn called but not implemented for 1.21.130");
}

// Stub implementation for buildBlock - needs signature resolution
void GameMode::buildBlock(Vec3i* blockPos, uint8_t face, bool useBlockSide) {
    // TODO: Resolve through signature scanning in 1.21.130
    // For now, log and skip to prevent linker errors
    logF("[GameMode] buildBlock called but not implemented for 1.21.130");
}
