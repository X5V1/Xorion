#pragma once
#include "Module.h"

class AutoCrystal : public IModule {
public:
    AutoCrystal();
    ~AutoCrystal();

    virtual const char* getModuleName() override;
    virtual void onTick(GameMode* gm) override;

private:
    int tickCounter = 0;
    bool torchPlaced = false;
    vec3_ti torchPos;
};
