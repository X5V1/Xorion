// Xorion/Module/Modules/InventoryViewer.h
#pragma once
#include "Module.h"
#include <vector>
#include "../../SDK/C_Entity.h"

class InventoryViewer : public IModule {
private:
    bool playersOnly = true;
    float scale = 1.0f;
    float x = 10.0f;
    float y = 80.0f;
    C_Entity* target = nullptr;

    void findTarget();
    void drawInventory(C_Entity* ent);

public:
    InventoryViewer();
    ~InventoryViewer();

    virtual const char* getModuleName() override;
    virtual void onTick(C_GameMode* gm) override;
    virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
};
