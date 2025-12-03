// Xorion/Module/Modules/DVDLogo.h
#pragma once
#include "Module.h"

class DVDLogo : public IModule {
private:
    float x = 120.0f;
    float y = 120.0f;
    float vx = 1.6f;
    float vy = 1.2f;
    float scale = 1.0f;
    int w = 100; // canvas width approx
    int h = 60;  // canvas height approx

public:
    DVDLogo();
    ~DVDLogo();

    virtual const char* getModuleName() override;
    virtual void onTick(C_GameMode* gm) override;
    virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
};
