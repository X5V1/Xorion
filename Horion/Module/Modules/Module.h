#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include "../Utils/Colors.h"   // color constants for modules that draw or log
#include "../Utils/Utils.h"    // common helpers used by some modules (CallVFunc etc.)
#include "../../../SDK/GameMode.h"  // For C_GameMode
#include "../../../SDK/MoveInputHandler.h"  // For onMove hook
#include "../../../SDK/MinecraftUIRenderContext.h"  // For render hooks

// Forward declarations (use struct consistently across SDK)
struct Packet;
class Player; // Player is a class
struct GameMode;

// Small POD for UI/layout positioning
struct ModulePos {
    float x = 0.0f;
    float y = 0.0f;
};

// Keybind type (fits Windows VK codes)
using KeyBind = uint32_t;

// Module categories (adjust as needed)
enum class Category : uint8_t {
    COMBAT = 0,
    VISUAL,
    MOVEMENT,
    PLAYER,
    WORLD,
    MISC,
    CLIENT
};

// Base module interface used by all modules.
// Declares the hooks modules expect to override.
class IModule {
public:
    IModule(KeyBind keybind = 0, Category category = Category::MISC, const std::string& description = "")
        : keybind(keybind), category(category), description(description) {}
    virtual ~IModule() = default;

    // Identification
    virtual const char* getModuleName() { return "Unnamed"; }
    virtual const char* getRawModuleName() { return getModuleName(); }
    virtual const std::string& getDescription() const { return description; }

    // Auto-start behavior
    virtual bool allowAutoStart() { return true; }

    // Lifecycle
    virtual void onEnable() {}
    virtual void onDisable() {}

    // Core hooks (unified signatures)
    virtual void onTick(C_GameMode* gm) {}
    virtual void onPlayerTick(Player* player) {}
    virtual void onSendPacket(Packet* packet) {}

    // Render hooks (UI)
    virtual void onPreRender(MinecraftUIRenderContext* ctx) {}
    virtual void onPostRender(MinecraftUIRenderContext* ctx) {}
    virtual void onLevelRender() {}

    // Input / keybind handling
    virtual void onKeyUpdate(uint32_t key, bool isDown) {}

    // Optional: settings registration stubs (wire these into your ClickGUI / settings manager)
    virtual void registerBoolSetting(const char* name, bool* ptr, bool defaultVal) {}
    virtual void registerIntSetting(const char* name, int* ptr, int defaultVal, int minVal, int maxVal) {}
    virtual void registerFloatSetting(const char* name, float* ptr, float defaultVal, float minVal, float maxVal) {}

    // Returns true if this module should still receive hooks when disabled
    virtual bool callWhenDisabled() { return false; }

    // Toggle helpers
    bool isEnabled() const { return enabled; }
    void setEnabled(bool e) {
        if (enabled == e) return;
        enabled = e;
        if (enabled) onEnable(); else onDisable();
    }
    void toggle() { setEnabled(!enabled); }

    // Metadata
    Category getCategory() const { return category; }
    KeyBind getKeybind() const { return keybind; }
    void setKeybind(KeyBind kb) { keybind = kb; }

    // Position for HUD/arraylist layout
    ModulePos& getModulePos() { return pos; }
    void setModulePos(float x, float y) { pos.x = x; pos.y = y; }

protected:
    bool enabled = false;
    KeyBind keybind = 0;
    Category category = Category::MISC;
    std::string description;
    ModulePos pos{};
};
