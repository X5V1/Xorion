#include "LigmaNuts.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/LocalPlayer.h"
#include "../../SDK/Entity.h"
#include "../../Utils/Target.h"

LigmaNuts::LigmaNuts() 
    : IModule(0x0, Category::PLAYER, "Ligma nuts joke spammer") {
}

LigmaNuts::~LigmaNuts() {}

const char* LigmaNuts::getModuleName() {
    return "LigmaNuts";
}

void LigmaNuts::onEnable() {
    lastMessage = std::chrono::steady_clock::now();
    pickNextDelay();
}

void LigmaNuts::pickNextDelay() {
    // Random delay between 20s (20000 ms) and 10min (600000 ms)
    int minMs = 20000;
    int maxMs = 600000;
    nextDelayMs = minMs + (rand() % (maxMs - minMs + 1));
}

void LigmaNuts::sendCompliment() {
    // TODO: Implement actual compliment sending logic
    // For now, just log that we attempted to send
    LocalPlayer* lp = g_Data.getLocalPlayer();
    if (!lp) return;
    // g_Data.getGuiData()->displayClientMessageF("[LigmaNuts] Compliment sent!");
}

// TODO: sendJoke method not implemented
/*
void LigmaNuts::sendJoke() {
    LocalPlayer* lp = g_Data.getLocalPlayer();
    if (!lp) return;

    Entity* nearest = nullptr;
    float bestDist = 12.0f; // only nearby players
    g_Data.forEachEntity([&](Entity* ent, bool) {
        if (!ent || ent == lp) return;
        if (!ent->isPlayer()) return;
        float d = ent->getPos()->dist(lp->getPos());
        if (d < bestDist) { bestDist = d; nearest = ent; }
    });

    if (!nearest) return;

    // Pick a joke
    // std::string msg = jokes[rand() % jokes.size()];
    std::string msg = "ligma";

    // Construct whisper command (Bedrock uses /tell or /msg)
    std::string command = std::string("/tell ") + nearest->getNameTag()->getText() + " " + msg;
    g_Data.getGuiData()->displayClientMessageF(("[LigmaNuts] " + command).c_str()); // local feedback
    // g_Data.getClientInstance()->minecraft->sendChatMessage(command); // TODO: sendChatMessage not available
}
*/

void LigmaNuts::onTick(C_GameMode* gm) {
    if (!g_Data.isInGame()) return;

    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastMessage).count();

    if (elapsed >= nextDelayMs) {
        sendCompliment();
        lastMessage = now;
        pickNextDelay();
    }
}

