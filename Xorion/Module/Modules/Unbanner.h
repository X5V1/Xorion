#pragma once
#include "Module.h"
#include <string>

// Forward declaration
struct TextHolder;

class Unbanner : public IModule {
public:
    // Settings
    bool spoofName = true;       // Enable username spoofing
    bool spoofDeviceId = true;   // Enable Device ID spoofing
    bool spoofXuid = true;       // Enable Xbox Live ID (XUID) spoofing
    bool spoofIP = true;         // Enable IP address spoofing (VPN-like feature)
    bool showButton = true;      // Show UI button

    // State
    std::string spoofedUsername;  // The spoofed username (word + 3 numbers)
    std::string spoofedDeviceId;  // The spoofed Device ID (UUID format)
    std::string spoofedXuid;      // The spoofed Xbox Live ID (numeric)
    std::string spoofedIP;        // The spoofed IP address
    bool usernameGenerated = false;
    bool deviceIdGenerated = false;
    bool xuidGenerated = false;
    bool ipGenerated = false;
    TextHolder* fakeNameHolder = nullptr;     // Holds the TextHolder for username
    TextHolder* fakeDeviceIdHolder = nullptr; // Holds the TextHolder for Device ID
    TextHolder* fakeXuidHolder = nullptr;     // Holds the TextHolder for XUID
    TextHolder* fakeIPHolder = nullptr;       // Holds the TextHolder for IP

    Unbanner();
    ~Unbanner();

    virtual const char* getModuleName() override;

    // Hooks
    void onEnable() override;
    void onDisable() override;
    void onTick(GameMode* gm) override;
    void onPostRender(MinecraftUIRenderContext* ctx) override;
    void onSendPacket(Packet* packet) override;

    // Generate spoofed identifiers
    void generateSpoofedUsername();
    void generateSpoofedDeviceId();
    void generateSpoofedXuid();
    void generateSpoofedIP();
    void generateAllSpoofedIds();

    // Get the current spoofed values
    const std::string& getSpoofedUsername() const;
    const std::string& getSpoofedDeviceId() const;
    const std::string& getSpoofedXuid() const;
    const std::string& getSpoofedIP() const;

private:
    // Render the "Try Unban Me" button
    void renderUnbanButton(MinecraftUIRenderContext* ctx);
    
    // Random word list for username generation
    static const char* wordList[];
    static const int wordListSize;
};
