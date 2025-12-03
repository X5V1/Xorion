#pragma once

#include <string>
#include <memory>
#include "Vec3.h"
#include "ItemStack.h"
#include "PlayerSupplies.h"
#include "Entity.h"
#include "Packet.h"

// High-level Player wrapper in Xorion style.
// Exposes a stable, feature‑rich interface to game player objects.
// Implementation in Player.cpp resolves vfunc indices / signatures at runtime.

class Player : public Entity {
public:
    explicit Player(void* mcPlayerPtr = nullptr);
    virtual ~Player();

    // Basic accessors
    std::string getName() const;
    ItemStack getHeldItem() const;
    Vec3 getPos() const;
    void setPos(const Vec3& pos);

    // Health / status
    float getHealth() const;
    void setHealth(float health);
    bool isSneaking() const;
    bool isSprinting() const;

    // Inventory / supplies
    PlayerSupplies* getSupplies() const;
    ItemStack* getSelectedItemStack() const;
    int getSelectedItemId() const;

    // Device / runtime helpers
    int getDeviceID() const;
    const char* getDeviceName() const;

    // Network / gameplay integration
    void sendNetworkPacket(Packet& packet);

    // Raw pointer access
    inline void* getMcPlayer() const { return mcPlayerPtr; }

protected:
    void* mcPlayerPtr = nullptr;

    // Resolved function pointers (populated in ensureIntegration)
    static void* (*s_getHeldItem_fn)(void* mcPlayer);
    static const char* (*s_getName_fn)(void* mcPlayer);
    static Vec3 (*s_getPos_fn)(void* mcPlayer);
    static void (*s_setPos_fn)(void* mcPlayer, const Vec3& pos);
    static float (*s_getHealth_fn)(void* mcPlayer);
    static void (*s_setHealth_fn)(void* mcPlayer, float);
    static void (*s_sendNetworkPacket_fn)(void* mcPlayer, Packet&);

    // Offsets (fall back to reading at offsets if vfuncs not available)
    static constexpr ptrdiff_t OFF_ITEM_PTR = 0x10;
    static constexpr ptrdiff_t OFF_ITEM_COUNT = 0x14;
    static constexpr ptrdiff_t OFF_AUX = 0x18;
    static constexpr ptrdiff_t OFF_SUPPLIES_PTR = 0x360; // example offset; adjust to target build
    static constexpr ptrdiff_t OFF_DEVICE_ID = 0x848;

    void ensureIntegration() const;
};
