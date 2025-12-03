#include "LigmaNuts.h"
#include "../../SDK/GameData.h"
#include "../../SDK/C_LocalPlayer.h"
#include "../../SDK/C_Entity.h"
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

void LigmaNuts::sendJoke() {
    C_LocalPlayer* lp = g_Data.getLocalPlayer();
    if (!lp) return;

    C_Entity* nearest = nullptr;
    float bestDist = 12.0f; // only nearby players
    g_Data.forEachEntity([&](C_Entity* ent, bool) {
        if (!ent || ent == lp) return;
        if (!ent->isPlayer()) return;
        float d = (*ent->getPos()).dist(*lp->getPos());
        if (d < bestDist) { bestDist = d; nearest = ent; }
    });

    if (!nearest) return;

    // Pick a joke
    std::string msg = jokes[rand() % jokes.size()];

    // Construct whisper command (Bedrock uses /tell or /msg)
    std::string command = std::string("/tell ") + nearest->getNameTag()->getText() + " " + msg;
    g_Data.getGuiData()->displayClientMessageF(("[LigmaNuts] " + command).c_str()); // local feedback
    g_Data.getClientInstance()->minecraft->sendChatMessage(command);
}

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
