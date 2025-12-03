#include "Player.h"
#include "PlayerSignatures.h"
#include "../Utils/Utils.h"
#include "../SDK/FindSignatures.h"
#include <vector>
#include <cstdio>
#include <cstring>

// Declarations of resolver helpers in Signatures.cpp
extern void* ResolvePlayerGetName();
extern void* ResolvePlayerGetHeldItem();
extern void* ResolvePlayerGetPos();
extern void* ResolvePlayerSetPos();
extern void* ResolvePlayerGetHealth();
extern void* ResolvePlayerSetHealth();
extern void* ResolvePlayerSendPacket();

extern void* ResolveLocalPlayerSwing();
extern void* ResolveLocalPlayerTurn();
extern void* ResolveLocalPlayerApplyTurn();
extern void* ResolveLocalPlayerGetGameMode();
extern void* ResolveLocalPlayerUnlock();

// Typedefs with calling conventions - tuned for Xorion style / MSVC x64 / typical game functions.
// On MSVC/x64 all non-member functions use the x64 calling convention. On 32-bit you may need __thiscall.
// We include explicit __fastcall/__thiscall for clarity for ports that require them.
using Player_getName_t = const char*(__cdecl*)(void*);           // usually plain pointer returns const char*
using Player_getHeldItem_t = void*(__cdecl*)(void*);
using Player_getPos_t = Vec3(__cdecl*)(void*);
using Player_setPos_t = void(__cdecl*)(void*, const Vec3&);
using Player_getHealth_t = float(__cdecl*)(void*);
using Player_setHealth_t = void(__cdecl*)(void*, float);
using Player_sendPacket_t = void(__cdecl*)(void*, Packet&);

// LocalPlayer typedefs
using LocalPlayer_swing_t = void(__cdecl*)(void*);
using LocalPlayer_turn_t = void(__cdecl*)(void*, Vec2*);
using LocalPlayer_applyTurn_t = void(__cdecl*)(void*, Vec2*);
using LocalPlayer_getGameMode_t = GameMode*(__cdecl*)(void*);
using LocalPlayer_unlock_t = void(__cdecl*)(void*);

// Resolved function pointers (initialized once)
static Player_getName_t       s_getName_fn = nullptr;
static Player_getHeldItem_t  s_getHeldItem_fn = nullptr;
static Player_getPos_t       s_getPos_fn = nullptr;
static Player_setPos_t       s_setPos_fn = nullptr;
static Player_getHealth_t    s_getHealth_fn = nullptr;
static Player_setHealth_t    s_setHealth_fn = nullptr;
static Player_sendPacket_t   s_sendNetworkPacket_fn = nullptr;

static LocalPlayer_swing_t   s_local_swing_fn = nullptr;
static LocalPlayer_turn_t    s_local_turn_fn = nullptr;
static LocalPlayer_applyTurn_t s_local_applyTurn_fn = nullptr;
static LocalPlayer_getGameMode_t s_local_getGameMode_fn = nullptr;
static LocalPlayer_unlock_t  s_local_unlock_fn = nullptr;

// Single resolver called on demand (idempotent)
static void ResolvePlayerSignaturesOnce() {
    static bool resolved = false;
    if (resolved) return;
    resolved = true;

    // Resolve Player functions using central Signatures resolver
    void* p;

    p = ResolvePlayerGetName();
    if (p) s_getName_fn = reinterpret_cast<Player_getName_t>(p);

    p = ResolvePlayerGetHeldItem();
    if (p) s_getHeldItem_fn = reinterpret_cast<Player_getHeldItem_t>(p);

    p = ResolvePlayerGetPos();
    if (p) s_getPos_fn = reinterpret_cast<Player_getPos_t>(p);

    p = ResolvePlayerSetPos();
    if (p) s_setPos_fn = reinterpret_cast<Player_setPos_t>(p);

    p = ResolvePlayerGetHealth();
    if (p) s_getHealth_fn = reinterpret_cast<Player_getHealth_t>(p);

    p = ResolvePlayerSetHealth();
    if (p) s_setHealth_fn = reinterpret_cast<Player_setHealth_t>(p);

    p = ResolvePlayerSendPacket();
    if (p) s_sendNetworkPacket_fn = reinterpret_cast<Player_sendPacket_t>(p);

    // LocalPlayer
    p = ResolveLocalPlayerSwing();
    if (p) s_local_swing_fn = reinterpret_cast<LocalPlayer_swing_t>(p);

    p = ResolveLocalPlayerTurn();
    if (p) s_local_turn_fn = reinterpret_cast<LocalPlayer_turn_t>(p);

    p = ResolveLocalPlayerApplyTurn();
    if (p) s_local_applyTurn_fn = reinterpret_cast<LocalPlayer_applyTurn_t>(p);

    p = ResolveLocalPlayerGetGameMode();
    if (p) s_local_getGameMode_fn = reinterpret_cast<LocalPlayer_getGameMode_t>(p);

    p = ResolveLocalPlayerUnlock();
    if (p) s_local_unlock_fn = reinterpret_cast<LocalPlayer_unlock_t>(p);

    // Log summary
    Utils::Log("[Player] Signature resolution complete: name=%p heldItem=%p getPos=%p setPos=%p getHealth=%p sendPacket=%p\n",
               (void*)s_getName_fn, (void*)s_getHeldItem_fn, (void*)s_getPos_fn, (void*)s_setPos_fn, (void*)s_getHealth_fn, (void*)s_sendNetworkPacket_fn);
    Utils::Log("[LocalPlayer] swing=%p turn=%p applyTurn=%p getGameMode=%p unlock=%p\n",
               (void*)s_local_swing_fn, (void*)s_local_turn_fn, (void*)s_local_applyTurn_fn, (void*)s_local_getGameMode_fn, (void*)s_local_unlock_fn);
}

// --- Player implementation (Xorion-style wrappers) ---

// Offsets used as validated fallbacks; keep in one place so you can tune for your build.
static constexpr ptrdiff_t OFF_SUPPLIES_PTR = OFF_SUPPLIES_PTR_DEFAULT;
static constexpr ptrdiff_t OFF_DEVICE_ID    = OFF_DEVICE_ID_DEFAULT;
static constexpr ptrdiff_t ITEM_OFF_PTR     = ITEM_RAW_OFF_PTR;
static constexpr ptrdiff_t ITEM_OFF_COUNT   = ITEM_RAW_OFF_COUNT;
static constexpr ptrdiff_t ITEM_OFF_AUX     = ITEM_RAW_OFF_AUX;
static constexpr ptrdiff_t ITEM_OFF_NAME    = ITEM_RAW_OFF_NAME;
static constexpr ptrdiff_t ITEM_OFF_TAG     = ITEM_RAW_OFF_TAG;
static constexpr ptrdiff_t LOCALPLAYER_GAMEMODE_PTR = LOCALPLAYER_GAMEMODE_OFF;

Player::Player(void* mcPlayerPtr) : mcPlayerPtr(mcPlayerPtr) {
    // Defer heavy resolution; but it's useful to kick it early if possible
    ResolvePlayerSignaturesOnce();
}

Player::~Player() = default;

void Player::ensureIntegration() const {
    ResolvePlayerSignaturesOnce();
}

std::string Player::getName() const {
    ensureIntegration();
    if (!mcPlayerPtr) return std::string();
    if (s_getName_fn) {
        const char* n = s_getName_fn(mcPlayerPtr);
        return n ? std::string(n) : std::string();
    }
    // No signature; return empty
    return std::string();
}

ItemStack Player::getHeldItem() const {
    ensureIntegration();
    if (!mcPlayerPtr) return ItemStack{};
    if (s_getHeldItem_fn) {
        void* raw = s_getHeldItem_fn(mcPlayerPtr);
        return ItemStack(raw);
    }
    // Fallback: read from common offset
    void* rawPtr = nullptr;
    auto ptr = reinterpret_cast<uint8_t*>(mcPlayerPtr) + ITEM_OFF_PTR;
    rawPtr = *reinterpret_cast<void**>(ptr);
    return ItemStack(rawPtr);
}

Vec3 Player::getPos() const {
    ensureIntegration();
    if (!mcPlayerPtr) return Vec3{};
    if (s_getPos_fn) {
        return s_getPos_fn(mcPlayerPtr);
    }
    // Fallback: call base Entity::getPos if available
    Vec3* p = nullptr;
    try {
        p = Entity::getPos();
    } catch (...) {
        p = nullptr;
    }
    return p ? *p : Vec3{};
}

void Player::setPos(const Vec3& pos) {
    ensureIntegration();
    if (!mcPlayerPtr) return;
    if (s_setPos_fn) {
        s_setPos_fn(mcPlayerPtr, pos);
        return;
    }
    // fallback: attempt common CallVFunc index (Xorion candidate)
    Utils::CallVFunc</*idx*/ 100, void, const Vec3&>(mcPlayerPtr, pos);
}

float Player::getHealth() const {
    ensureIntegration();
    if (!mcPlayerPtr) return 0.0f;
    if (s_getHealth_fn) return s_getHealth_fn(mcPlayerPtr);
    // fallback: read at a plausible offset
    float* hp = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(mcPlayerPtr) + 0x4B0);
    return hp ? *hp : 0.0f;
}

void Player::setHealth(float health) {
    ensureIntegration();
    if (!mcPlayerPtr) return;
    if (s_setHealth_fn) { s_setHealth_fn(mcPlayerPtr, health); return; }
    float* hp = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(mcPlayerPtr) + 0x4B0);
    if (hp) *hp = health;
}

bool Player::isSneaking() const {
    return Entity::getStatusFlag(SNEAKING);
}

bool Player::isSprinting() const {
    return Entity::getStatusFlag(SPRINTING);
}

PlayerSupplies* Player::getSupplies() const {
    if (!mcPlayerPtr) return nullptr;
    void* ptr = *reinterpret_cast<void**>(reinterpret_cast<uintptr_t>(mcPlayerPtr) + OFF_SUPPLIES_PTR);
    return reinterpret_cast<PlayerSupplies*>(ptr);
}

ItemStack* Player::getSelectedItemStack() const {
    PlayerSupplies* s = getSupplies();
    if (!s) return nullptr;
    return s->getHotbarSlot(s->selectedHotbarSlot);
}

int Player::getSelectedItemId() const {
    ItemStack* s = getSelectedItemStack();
    if (!s || s->isNull()) return 0;
    Item* it = s->getItem();
    if (!it) return 0;
    return it->itemId;
}

int Player::getDeviceID() const {
    if (!mcPlayerPtr) return -1;
    return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(mcPlayerPtr) + OFF_DEVICE_ID);
}

const char* Player::getDeviceName() const {
    int id = getDeviceID();
    switch (id) {
    case -1: return "Unknown";
    case 1: return "Android";
    case 2: return "iOS";
    case 3: return "OSX";
    case 4: return "Amazon";
    case 5: return "GearVR";
    case 6: return "Hololens";
    case 7: return "Win10";
    case 8: return "WIN32";
    case 9: return "Dedicated";
    case 10: return "TVOS";
    case 11: return "PlayStation";
    case 12: return "Nintendo";
    case 13: return "Xbox";
    case 14: return "WindowsPhone";
    case 15: return "Linux";
    default: return "Unknown";
    }
}

void Player::sendNetworkPacket(Packet& packet) {
    ensureIntegration();
    if (!mcPlayerPtr) return;
    if (s_sendNetworkPacket_fn) {
        s_sendNetworkPacket_fn(mcPlayerPtr, packet);
        return;
    }
    // fallback
    Utils::CallVFunc</*idx*/ 200, void, Packet&>(mcPlayerPtr, packet);
}

// --- LocalPlayer helpers ---

LocalPlayer::LocalPlayer(void* mcPlayerPtr) : Player(mcPlayerPtr) {
    // signatures already resolved in Player::ResolvePlayerSignaturesOnce call if not, call now
    ResolvePlayerSignaturesOnce();
}

LocalPlayer::~LocalPlayer() = default;

void LocalPlayer::swingArm() {
    if (!mcPlayerPtr) return;
    if (s_local_swing_fn) { s_local_swing_fn(mcPlayerPtr); return; }
    // fallback vfunc candidate
    Utils::CallVFunc<Signatures::VF_Player_swing_fallback, void>(mcPlayerPtr);
}

void LocalPlayer::localPlayerTurn(Vec2* viewAngles) {
    if (!mcPlayerPtr || !viewAngles) return;
    if (s_local_turn_fn) { s_local_turn_fn(mcPlayerPtr, viewAngles); return; }
    using TurnFn = void(__thiscall*)(void*, Vec2*);
    static TurnFn fallback = reinterpret_cast<TurnFn>(FindSignature("48 8B 05 ? ? ? ? 48 8B 80 ? ? ? ? 48 8B D1 E8 ? ? ? ?"));
    if (fallback) fallback(mcPlayerPtr, viewAngles);
}

void LocalPlayer::applyTurnDelta(Vec2* viewAngleDelta) {
    if (!mcPlayerPtr || !viewAngleDelta) return;
    if (s_local_applyTurn_fn) { s_local_applyTurn_fn(mcPlayerPtr, viewAngleDelta); return; }
    using ApplyFn = void(__thiscall*)(void*, Vec2*);
    static ApplyFn fallback = reinterpret_cast<ApplyFn>(FindSignature("LOCALPLAYER_applyTurn_fallback_sig"));
    if (fallback) fallback(mcPlayerPtr, viewAngleDelta);
}

GameMode* LocalPlayer::getGameMode() const {
    if (!mcPlayerPtr) return nullptr;
    if (s_local_getGameMode_fn) return s_local_getGameMode_fn(mcPlayerPtr);
    return *reinterpret_cast<GameMode**>(reinterpret_cast<uintptr_t>(mcPlayerPtr) + LOCALPLAYER_GAMEMODE_PTR);
}

void LocalPlayer::unlockAchievements() {
    if (!mcPlayerPtr) return;
    if (s_local_unlock_fn) { s_local_unlock_fn(mcPlayerPtr); return; }
    Utils::CallVFunc<Signatures::VF_LocalPlayer_unlockAchievements_fallback, void>(mcPlayerPtr);
}
