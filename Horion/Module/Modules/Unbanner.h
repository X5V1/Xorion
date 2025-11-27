#pragma once
#include "Module.h"
#include <string>

class Unbanner : public IModule {
public:
    // Settings
    bool autoReconnect = true;
    int retryDelay = 60;

    // State
    int tickCounter = 0;
    bool showButton = true;

    Unbanner();
    ~Unbanner();

    virtual const char* getModuleName() override;

    // Hooks
    void onEnable() override;
    void onDisable() override;
    void onTick(GameMode* gm) override;
    void onPostRender(MinecraftUIRenderContext* ctx) override;

private:
    // Render the "Try Unban Me" button
    void renderUnbanButton(MinecraftUIRenderContext* ctx);
    // Attempt to reconnect/unban
    void tryUnban();
};
