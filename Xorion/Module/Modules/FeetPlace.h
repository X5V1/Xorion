#pragma once
#include "Module.h"
#include "LocalPlayer.h"
#include "IWorld.h"

class FeetPlace : public Module {
public:
    FeetPlace() 
        : Module("FeetPlace", "World", "Places obsidian at your feet directly in front of you") {}

    void onEnable() override {}
    void onDisable() override {}
    void onTick(GameMode* gm) override;
};
