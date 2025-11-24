#include "LocalPlayer.h"
#include "../Utils/Utils.h"
#include "../SDK/FindSignatures.h"
#include <vector>

static void* findAnySignatureLocal(const std::vector<const char*>& patterns) {
    for (auto p : patterns) {
        void* r = FindSignature(p);
        if (r) return r;
    }
    return nullptr;
}

// Static defs
void (*LocalPlayer::s_swingArm_fn)(void*) = nullptr;
void (*LocalPlayer::s_localPlayerTurn_fn)(void*, Vec2*) = nullptr;
void (*LocalPlayer::s_applyTurnDelta_fn)(void*, Vec2*) = nullptr;
GameMode* (*LocalPlayer::s_getGameMode_fn)(void*) = nullptr;

LocalPlayer::LocalPlayer(void* mcPlayerPtr) : Player(mcPlayerPtr) {
    ensureLocalIntegration();
}

LocalPlayer::~LocalPlayer() = default;

void LocalPlayer::ensureLocalIntegration() const {
    if (!s_swingArm_fn) {
        std::vector<const char*> pats = {
            "40 53 48 83 EC ? 48 8B D9 48 8B 0D ? ? ? ? 48 8B 01 FF 50 ?",
            "48 89 5C 24 ? 48 89 7C 24 ? 57 48 83 EC ? 8B F9 E8 ? ? ? ?"
        };
        s_swingArm_fn = reinterpret_cast<void(*)(void*)>(findAnySignatureLocal(pats));
    }
    if (!s_localPlayerTurn_fn) {
        std::vector<const char*> pats = {
            "48 8B 58 68 48 83 EC ? 48 8B F1 48 8B 4A ? E8 ? ? ? ?",
            "48 8B 05 ? ? ? ? 48 8B 80 ? ? ? ? 48 8B D1 E8 ? ? ? ?"
        };
        s_localPlayerTurn_fn = reinterpret_cast<void(*)(void*, Vec2*)>(findAnySignatureLocal(pats));
    }
    if (!s_applyTurnDelta_fn) {
        std::vector<const char*> pats = {
            "48 89 5C 24 ? 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 8B F9 48 8B 01 48 8B 68 ?"
        };
        s_applyTurnDelta_fn = reinterpret_cast<void(*)(void*, Vec2*)>(findAnySignatureLocal(pats));
    }
    if (!s_getGameMode_fn) {
        std::vector<const char*> pats = {
            "48 8B 81 ? ? ? ? 48 8B 48 ? 48 8B 01 FF 50 ?",
            "48 8B 05 ? ? ? ? 48 8B 50 ? 48 8B 88 ? ? ? ? 48 85 C9"
        };
        s_getGameMode_fn = reinterpret_cast<GameMode*(*)(void*)>(findAnySignatureLocal(pats));
    }
}

void LocalPlayer::swingArm() {
    if (!mcPlayerPtr) return;
    ensureLocalIntegration();
    if (s_swingArm_fn) { s_swingArm_fn(mcPlayerPtr); return; }
    // fallback vfunc (candidate)
    Utils::CallVFunc<56, void>(mcPlayerPtr);
}

void LocalPlayer::localPlayerTurn(Vec2* viewAngles) {
    if (!mcPlayerPtr || !viewAngles) return;
    ensureLocalIntegration();
    if (s_localPlayerTurn_fn) { s_localPlayerTurn_fn(mcPlayerPtr, viewAngles); return; }
    using TurnFn = void(__thiscall*)(void*, Vec2*);
    static TurnFn fallback = reinterpret_cast<TurnFn>(FindSignature("LOCALPLAYER_turn_fallback_sig"));
    if (fallback) fallback(mcPlayerPtr, viewAngles);
}

void LocalPlayer::applyTurnDelta(Vec2* viewAngleDelta) {
    if (!mcPlayerPtr || !viewAngleDelta) return;
    ensureLocalIntegration();
    if (s_applyTurnDelta_fn) { s_applyTurnDelta_fn(mcPlayerPtr, viewAngleDelta); return; }
    using ApplyFn = void(__thiscall*)(void*, Vec2*);
    static ApplyFn fallback = reinterpret_cast<ApplyFn>(FindSignature("LOCALPLAYER_applyTurn_fallback_sig"));
    if (fallback) fallback(mcPlayerPtr, viewAngleDelta);
}

GameMode* LocalPlayer::getGameMode() const {
    ensureLocalIntegration();
    if (!mcPlayerPtr) return nullptr;
    if (s_getGameMode_fn) return s_getGameMode_fn(mcPlayerPtr);
    // fallback: read from offset
    return *reinterpret_cast<GameMode**>(reinterpret_cast<uintptr_t>(mcPlayerPtr) + 0xEF8);
}

void LocalPlayer::unlockAchievements() {
    if (!mcPlayerPtr) return;
    ensureLocalIntegration();
    // try vfunc fallback candidate
    Utils::CallVFunc<220, void>(mcPlayerPtr);
}
