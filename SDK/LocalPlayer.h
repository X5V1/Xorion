// LocalPlayer.h
#pragma once
#include <string>
#include "Player.h"

class GameMode;
class ClientInstance;

class LocalPlayer : public Player {
public:
    explicit LocalPlayer(void* mcLocalPlayerPtr = nullptr);
    ~LocalPlayer();

    void sendChatMessage(const std::string& msg);
    void attackEntity(Entity* entity);
    void jump();

    bool isSneaking() const;
    void setSneaking(bool sneaking);

    GameMode*     getGameMode() const;
    ClientInstance* getClientInstance() const;

private:
    bool sneaking;

    static void (*ClientInstance_sendChat_fn)(ClientInstance* ci, const char* msg);
    static void (*GameMode_attack_fn)(GameMode* gm, Entity* entity);
    static void (*LocalPlayer_jump_fn)(void* mcLocalPlayer);
    static bool (*LocalPlayer_isSneaking_fn)(void* mcLocalPlayer);
    static void (*LocalPlayer_setSneaking_fn)(void* mcLocalPlayer, bool sneaking);

    void ensureIntegration() const;
};
