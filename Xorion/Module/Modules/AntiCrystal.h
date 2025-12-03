// Xorion/Module/Modules/AntiCrystal.h
#pragma once
#include "Module.h"
#include "../../SDK/Entity.h"

class AntiCrystal : public IModule {
private:
    float range = 5.0f;
    bool autoBreak = true;
    bool dodge = true;

    void actOnCrystals(GameMode* gm);

public:
    AntiCrystal();
    ~AntiCrystal();

    virtual const char* getModuleName() override;
    virtual void onTick(GameMode* gm) override;
};
