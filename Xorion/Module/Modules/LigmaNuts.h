#pragma once
#include "Module.h"
#include <vector>
#include <string>
#include <chrono>

class LigmaNuts : public IModule {
private:
    std::vector<std::string> Jokes = {
        "Ligma nuts!",
    };

    std::chrono::steady_clock::time_point lastMessage;
    int nextDelayMs = 0; // randomized delay in ms

    void pickNextDelay();
    void sendCompliment();

public:
    LigmaNuts();
    ~LigmaNuts();

    virtual const char* getModuleName() override;
    virtual void onEnable() override;
    virtual void onTick(C_GameMode* gm) override;
};
