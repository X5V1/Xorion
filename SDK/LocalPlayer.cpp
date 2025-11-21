// LocalPlayer.cpp
#include "LocalPlayer.h"
#include <stdexcept>
#include "Utils/Logger.h"
#include "SDK/GameMode.h"
#include "SDK/ClientInstance.h"
#include "GameData.h"

void (*LocalPlayer::ClientInstance_sendChat_fn)(ClientInstance*, const char*) = nullptr;
void (*LocalPlayer::GameMode_attack_fn)(GameMode*, Entity*) = nullptr;
void (*LocalPlayer::LocalPlayer_jump_fn)(void*) = nullptr;
bool (*LocalPlayer::LocalPlayer_isSneaking_fn)(void*) = nullptr;
void (*LocalPlayer::LocalPlayer_setSneaking_fn)(void*, bool) = nullptr;

LocalPlayer::LocalPlayer(void* mcLocalPlayerPtr)
    : Player(mcLocalPlayerPtr), sneaking(false) {}

LocalPlayer::~LocalPlayer() {}

void LocalPlayer::ensureIntegration() const {
    if (!mcPlayerPtr ||
        !GameMode_attack_fn ||
        !ClientInstance_sendChat_fn) {
        logF("[LocalPlayer] Missing integration. Ensure InitSDK() succeeded for 1.21.121.");
        throw std::runtime_error("LocalPlayer SDK not initialized");
    }
}

GameMode* LocalPlayer::getGameMode() const { return g_Data.getCGameMode(); }
ClientInstance* LocalPlayer::getClientInstance() const { return g_Data.getClientInstance(); }

void LocalPlayer::sendChatMessage(const std::string& msg) {
    ensureIntegration();
    ClientInstance_sendChat_fn(getClientInstance(), msg.c_str());
}

void LocalPlayer::attackEntity(Entity* entity) {
    ensureIntegration();
    GameMode_attack_fn(getGameMode(), entity);
}

void LocalPlayer::jump() {
    ensureIntegration();
    if (!LocalPlayer_jump_fn) {
        logF("[LocalPlayer] jump signature missing for 1.21.121");
        return;
    }
    LocalPlayer_jump_fn(mcPlayerPtr);
}

bool LocalPlayer::isSneaking() const {
    ensureIntegration();
    if (!LocalPlayer_isSneaking_fn) {
        logF("[LocalPlayer] isSneaking signature missing for 1.21.121");
        return false;
    }
    return LocalPlayer_isSneaking_fn(mcPlayerPtr);
}

void LocalPlayer::setSneaking(bool s) {
    ensureIntegration();
    if (!LocalPlayer_setSneaking_fn) {
        logF("[LocalPlayer] setSneaking signature missing for 1.21.121");
        return;
    }
    LocalPlayer_setSneaking_fn(mcPlayerPtr, s);
}
