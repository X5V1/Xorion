// Xorion/Module/Modules/AntiEffect.h
#pragma once
#include "Module.h"
#include <vector>

class AntiEffect : public IModule {
private:
    bool clearBlindness = true;
    bool clearSlowness = true;
    bool clearWeakness = true;
    bool clearPoison = true;
    bool clearMiningFatigue = true;

    void purgeEffects();

public:
    AntiEffect();
    ~AntiEffect();

    virtual const char* getModuleName() override;
    virtual void onTick(C_GameMode* gm) override;
};
