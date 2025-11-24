#pragma once

#include "Player.h"
#include "Vec2.h"

// LocalPlayer extends Player with client-side operations used by modules (swing, turn, game mode access)
class LocalPlayer : public Player {
public:
    explicit LocalPlayer(void* mcPlayerPtr = nullptr);
    virtual ~LocalPlayer();

    // Client-side actions
    void swingArm();
    void localPlayerTurn(Vec2* viewAngles);
    void applyTurnDelta(Vec2* viewAngleDelta);

    // Accessors
    GameMode* getGameMode() const;
    void unlockAchievements();

protected:
    // Resolved function pointers for LocalPlayer specific calls
    static void (*s_swingArm_fn)(void*);
    static void (*s_localPlayerTurn_fn)(void*, Vec2*);
    static void (*s_applyTurnDelta_fn)(void*, Vec2*);
    static GameMode* (*s_getGameMode_fn)(void*);

    void ensureLocalIntegration() const;
};
