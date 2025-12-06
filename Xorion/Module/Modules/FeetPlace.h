#pragma once
#include "Module.h"
#include "../../../SDK/LocalPlayer.h"

class FeetPlace : public IModule {
public:
    FeetPlace() 
        : IModule(0, Category::WORLD, "Places obsidian at your feet directly in front of you") {}

    virtual const char* getModuleName() override { return "FeetPlace"; }
    virtual void onTick(GameMode* gm) override;
};
