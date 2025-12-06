#pragma once
#include "Module.h"
#include <string>
#include <vector>
#include <set>
#include <chrono>
#include <mutex>

// Forward declarations
struct Packet;
struct TextPacket;
struct CommandRequestPacket;
struct DisconnectPacket;

class AntiCheatBlocker : public IModule {
public:
    AntiCheatBlocker();
    ~AntiCheatBlocker();

    virtual const char* getModuleName() override;

    // Hooks
    void onTick(GameMode* gm) override;
    void onSendPacket(Packet* packet) override;

    // Override to run even when "disabled" (always-on background protection)
    bool callWhenDisabled() override { return true; }

    // Check if anti-cheat was detected
    bool wasAntiCheatDetected() const { return detectionTriggered; }
    
    // Get learned anti-cheats count
    size_t getLearnedAntiCheatsCount() const;

private:
    // Built-in list of known anti-cheat plugin names/patterns
    static const std::vector<std::string> builtInAntiCheats;
    
    // List of known ban command patterns to intercept
    static const std::vector<std::string> banCommandPatterns;
    
    // Suspicious keywords that indicate possible anti-cheat
    static const std::vector<std::string> suspiciousKeywords;
    
    // Learned anti-cheats (loaded from and saved to file)
    std::set<std::string> learnedAntiCheats;
    std::mutex learnedMutex;
    bool learnedDataModified = false;

    // Detection state
    bool detectionTriggered = false;
    bool hasKickedPlayer = false;
    int detectionCount = 0;
    int aggressiveBlockCount = 0;
    std::chrono::steady_clock::time_point lastCheckTime;
    std::chrono::steady_clock::time_point lastSaveTime;
    
    // Aggressive mode state
    bool aggressiveModeActive = false;
    int suspiciousActivityScore = 0;

    // Detection methods
    bool detectAntiCheatInMessage(const std::string& message);
    bool detectBanCommand(const std::string& command);
    bool detectSuspiciousActivity(const std::string& message);
    std::string extractPotentialAntiCheatName(const std::string& message);
    
    // Learning system
    void learnNewAntiCheat(const std::string& name);
    void loadLearnedAntiCheats();
    void saveLearnedAntiCheats();
    std::wstring getLearnedDataFilePath();
    
    // Protection actions
    void triggerSafeDisconnect();
    void triggerAggressiveProtection();
    void interceptBanCommand(Packet* packet);
    void blockAllSuspiciousPackets(Packet* packet);
    
    // Utility
    std::string toLower(const std::string& str);
    std::string sanitizeName(const std::string& name);
    bool isValidAntiCheatName(const std::string& name);
};
