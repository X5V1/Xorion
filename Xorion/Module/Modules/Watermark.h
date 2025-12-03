// Xorion/Module/Modules/Watermark.h
#pragma once
#include "Module.h"

class Watermark : public IModule {
private:
    float x = 6.0f;
    float y = 6.0f;
    float scale = 1.0f;
    bool rainbow = true;
    std::string text = "Xorion";

public:
    Watermark();
    ~Watermark();

    virtual const char* getModuleName() override;
    virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
};
