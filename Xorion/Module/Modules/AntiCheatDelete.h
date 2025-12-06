#pragma once
#include "Module.h"
#include <string>
#include <vector>
#include <chrono>
#include <mutex>

// Forward declarations
struct Packet;
struct TextPacket;
struct CommandRequestPacket;

class AntiCheatBlocker;

class AntiCheatDelete : public IModule {
public:
    AntiCheatDelete();
    ~AntiCheatDelete();

    virtual const char* getModuleName() override;

    // Hooks
    void onTick(GameMode* gm) override;
    void onSendPacket(Packet* packet) override;
    void onEnable() override;
    void onDisable() override;

    // Override to run even when "disabled" (always-on background protection)
    bool callWhenDisabled() override { return true; }

    // Get current deletion mode
    enum class DeletionMode {
        SERVER_WIDE,      // Trying to delete anticheat for whole server
        PLAYER_ONLY,      // Trying to delete anticheat for player only
        BLOCKER_FALLBACK  // Fell back to AntiCheatBlocker
    };
    
    DeletionMode getCurrentMode() const { return currentMode; }
    int getAttemptCount() const { return attemptCount; }
    bool isAggressivelyRetrying() const { return aggressiveRetryActive; }

private:
    // Built-in list of anti-cheat plugin command patterns to try deleting
    static const std::vector<std::string> serverDeleteCommands;
    static const std::vector<std::string> playerDeleteCommands;
    static const std::vector<std::string> antiCheatPluginNames;
    
    // State tracking
    DeletionMode currentMode = DeletionMode::SERVER_WIDE;
    int attemptCount = 0;
    int maxServerAttempts = 50;      // Very aggressive - try many times
    int maxPlayerAttempts = 30;      // Also aggressive
    int currentCommandIndex = 0;
    
    // Timing
    std::chrono::steady_clock::time_point lastAttemptTime;
    std::chrono::steady_clock::time_point startTime;
    int attemptIntervalMs = 100;     // Aggressive - 100ms between attempts
    int maxRetryDurationSeconds = 120;  // Try for up to 2 minutes
    
    // Aggressive retry state
    bool aggressiveRetryActive = false;
    int aggressiveRetryCount = 0;
    int maxAggressiveRetries = 200;  // Very aggressive retries
    
    // Success detection
    bool deletionSuccessful = false;
    bool hasTriggeredBlocker = false;
    
    std::mutex stateMutex;
    
    // Deletion attempt methods
    void attemptServerWideDeletion();
    void attemptPlayerOnlyDeletion();
    void fallbackToBlocker();
    
    // Send command helper
    void sendCommand(const std::string& command);
    void sendMultipleCommands(const std::vector<std::string>& commands);
    
    // Check for success/failure indicators
    bool detectDeletionSuccess(const std::string& message);
    bool detectDeletionFailure(const std::string& message);
    void handleResponse(const std::string& message);
    
    // Mode transition
    void transitionToPlayerMode();
    void transitionToBlockerMode();
    
    // Generate deletion commands dynamically
    std::vector<std::string> generateServerDeleteCommands(const std::string& pluginName);
    std::vector<std::string> generatePlayerDeleteCommands(const std::string& pluginName);
    
    // Utility
    std::string toLower(const std::string& str);
    std::string replacePluginName(const std::string& cmdTemplate, const std::string& pluginName);
    
    // Reference to AntiCheatBlocker for fallback
    AntiCheatBlocker* getAntiCheatBlocker();
};
