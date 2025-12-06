#pragma once
#include "Module.h"

class Dicker : public IModule {
private:
    int delay = 10; // ticks between each placement cycle
    int tickCounter = 0;
    int spacing = 4; // distance between each pattern

    void placePatternAt(GameMode* gm, const vec3_ti& basePos);

public:
    Dicker();
    ~Dicker();

    virtual const char* getModuleName() override;
    virtual void onTick(GameMode* gm) override;
};
