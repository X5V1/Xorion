#pragma once
#include "Module.h"
#include <string>
#include <vector>
#include <chrono>

// Forward declaration
struct TextHolder;
struct Packet;

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

    // Defense escalation state
    enum class DefenseLevel {
        NORMAL = 0,           // No threat detected
        BLOCKING_COMMANDS,    // Aggressively blocking ban/kick commands
        USERNAME_SPOOFING,    // Blocking failed, spoofing username
        LAST_RESORT_KICK      // All else failed, preparing to kick
    };
    DefenseLevel currentDefenseLevel = DefenseLevel::NORMAL;
    bool chatBlocked = false;              // Whether chat is currently blocked
    int blockedCommandCount = 0;           // Number of commands blocked
    int blockingFailureCount = 0;          // Number of times blocking has failed
    std::chrono::steady_clock::time_point defenseActivatedTime;  // When defense was activated
    std::chrono::steady_clock::time_point chatBlockedTime;       // When chat was blocked
    static constexpr int CHAT_BLOCK_DURATION_SECONDS = 30;       // Duration to block chat
    static constexpr int BLOCKING_FAILURE_THRESHOLD = 3;         // Failures before escalation
    static constexpr int SPOOF_FAILURE_THRESHOLD = 5;            // Failures before last resort

    Unbanner();
    ~Unbanner();

    virtual const char* getModuleName() override;

    // Hooks
    void onEnable() override;
    void onDisable() override;
    void onTick(GameMode* gm) override;
    void onPostRender(MinecraftUIRenderContext* ctx) override;
    void onSendPacket(Packet* packet) override;

    // Override to run even when "disabled" (always-on background protection)
    bool callWhenDisabled() override { return true; }

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

    // Defense escalation methods
    void activateDefense();
    void escalateDefense();
    void blockChat();
    void unblockChat();
    bool isChatBlocked() const { return chatBlocked; }
    DefenseLevel getDefenseLevel() const { return currentDefenseLevel; }
    void resetDefenseState();

private:
    // Render the "Try Unban Me" button
    void renderUnbanButton(MinecraftUIRenderContext* ctx);
    
    // Render chat blocked message
    void renderChatBlockedMessage(MinecraftUIRenderContext* ctx);
    
    // Ban/kick command detection and blocking
    bool detectBanKickCommand(const std::string& command);
    bool blockBanKickCommand(Packet* packet);
    
    // Known ban/kick command patterns
    static const std::vector<std::string> banKickPatterns;
    
    // Random word list for username generation
    static const char* wordList[];
    static const int wordListSize;
};
