#include "AntiCheatBlocker.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/Packet.h"
#include "../../../SDK/TextHolder.h"
#include "../../../Utils/Logger.h"
#include "../../Config/ConfigManager.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <regex>

// Built-in known anti-cheat plugin names and patterns (case-insensitive matching)
const std::vector<std::string> AntiCheatBlocker::builtInAntiCheats = {
    // Major Anti-Cheats
    "anticheat", "anti-cheat", "anticheats", "anti-cheats",
    "nocheatplus", "ncp", "nocheat",
    "aac", "advancedanticheat", "advanced-anti-cheat",
    "spartan", "spartanac", "spartan-anticheat",
    "matrix", "matrixac", "matrix-anticheat",
    "vulcan", "vulcanac", "vulcan-anticheat",
    "verus", "verusac", "verus-anticheat",
    "grim", "grimac", "grim-anticheat", "grimanticheat",
    "karhu", "karhuac", "karhu-anticheat",
    "intave", "intaveac", "intave-anticheat",
    "polar", "polarac", "polar-anticheat",
    "watchdog", "hypixel-watchdog", "hypixelwatchdog",
    "sentinel", "sentinelac", "sentinel-anticheat",
    // Game-wide Anti-Cheats
    "fairfight", "fair-fight",
    "battleye", "battle-eye", "be-anticheat",
    "easyanticheat", "easy-anti-cheat", "eac",
    "vac", "valve-anticheat", "valve-anti-cheat",
    "punkbuster", "punk-buster", "pb-anticheat",
    // Screen Share Tools
    "screenshare", "screen-share", "ss-tool", "sstool",
    "paladin", "paladinss", "paladin-ss",
    // Other Anti-Cheats
    "reflex", "reflexac", "reflex-anticheat",
    "horizon", "horizonac", "horizon-anticheat",
    "themis", "themisac", "themis-anticheat",
    "wraith", "wraithac", "wraith-anticheat",
    "badlion", "blc", "badlion-client", "badlionac",
    "hac", "hackcontrol", "hack-control",
    "negativity", "negativityac",
    "kauri", "kauriac", "kauri-anticheat",
    "godseye", "gods-eye", "godseye-anticheat",
    "hawk", "hawkac", "hawk-anticheat",
    "angel", "angelx", "angelac",
    "alice", "aliceac", "alice-anticheat",
    "artemis", "artemisac", "artemis-anticheat",
    "sonar", "sonarac", "sonar-anticheat",
    "arc", "arcac", "arc-anticheat",
    "guardian", "guardianac", "guardian-anticheat",
    "cerberus", "cerberusac", "cerberus-anticheat",
    "aegis", "aegisac", "aegis-anticheat",
    "warden", "wardenac", "warden-anticheat",
    "hydra", "hydraac", "hydra-anticheat",
    "phantom", "phantomac", "phantom-anticheat",
    "specter", "specterac", "specter-anticheat",
    "overwatch", "overwatchac"
};

// Known ban command patterns to intercept (exact command prefixes)
const std::vector<std::string> AntiCheatBlocker::banCommandPatterns = {
    // Standard ban commands
    "/ban ", "/banip ", "/ipban ", "/tempban ", "/tban ",
    "/kick ", "/kickip ", "/hardkick ",
    "/mute ", "/tempmute ", "/tmute ", "/globalmute ",
    "/jail ", "/tempjail ", "/unjail ",
    "/punish ", "/punishment ", "/autopunish ",
    "/warn ", "/warning ", "/strikes ",
    "/suspend ", "/suspension ",
    "/blacklist ", "/whitelist remove ",
    // Anti-cheat specific commands
    "/freeze ", "/ss ", "/screenshare ",
    "/notify ", "/alert ",
    "/report ", "/staffreport ",
    "/vanish ", "/v ", "/sv ",  // Staff vanish often precedes punishment
    // Server moderation
    "/execute ", "/sudo ",
    "/kill ", "/slay ", "/smite ",
    // Network bans
    "/gban ", "/globalban ", "/netban ", "/networkban "
};

// Suspicious keywords that may indicate anti-cheat activity
const std::vector<std::string> AntiCheatBlocker::suspiciousKeywords = {
    "cheating", "cheater", "hacking", "hacker", "hack detected",
    "cheat detected", "illegal modification", "unfair advantage",
    "client modification", "modified client", "hacked client",
    "flagged", "violation", "suspicious activity", "suspicious behavior",
    "auto-ban", "autoban", "automated ban", "automatic ban",
    "speed hack", "fly hack", "killaura", "reach hack",
    "autoclicker", "auto-clicker", "bhop", "scaffold",
    "invalid movement", "invalid packets", "packet manipulation",
    "you have been banned", "you are banned", "banned for",
    "kicked for", "removed from server", "connection terminated",
    "security check", "verification required", "captcha",
    "staff has been notified", "report submitted",
    "your account", "punishment", "infraction"
};

AntiCheatBlocker::AntiCheatBlocker()
    : IModule(0, Category::MISC, "Aggressive anti-cheat protection with learning. Always running in background.") {
    lastCheckTime = std::chrono::steady_clock::now();
    lastSaveTime = std::chrono::steady_clock::now();
    
    // Load previously learned anti-cheats
    loadLearnedAntiCheats();
    
    logF("[AntiCheatBlocker] Initialized with %zu built-in + %zu learned anti-cheats", 
         builtInAntiCheats.size(), learnedAntiCheats.size());
}

AntiCheatBlocker::~AntiCheatBlocker() {
    // Save learned data on destruction
    if (learnedDataModified) {
        saveLearnedAntiCheats();
    }
}

const char* AntiCheatBlocker::getModuleName() {
    return "AntiCheatBlocker";
}

std::wstring AntiCheatBlocker::getLearnedDataFilePath() {
    // Store in the same roaming folder as config
    try {
        if (configMgr != nullptr) {
            // Get roaming folder path using public accessor
            std::wstring roamingPath = configMgr->getRoamingFolder();
            return roamingPath + L"\\xorion_learned_anticheats.txt";
        }
    } catch (...) {
        logF("[AntiCheatBlocker] Failed to get roaming folder path");
    }
    return L"";
}

void AntiCheatBlocker::loadLearnedAntiCheats() {
    std::lock_guard<std::mutex> lock(learnedMutex);
    
    std::wstring filePath = getLearnedDataFilePath();
    if (filePath.empty()) return;
    
    try {
        if (std::filesystem::exists(filePath)) {
            std::ifstream file(filePath);
            if (file.is_open()) {
                std::string line;
                while (std::getline(file, line)) {
                    // Skip comments and empty lines
                    if (line.empty() || line[0] == '#') continue;
                    
                    std::string name = toLower(sanitizeName(line));
                    if (isValidAntiCheatName(name)) {
                        learnedAntiCheats.insert(name);
                    }
                }
                file.close();
                logF("[AntiCheatBlocker] Loaded %zu learned anti-cheats from file", learnedAntiCheats.size());
            }
        }
    } catch (const std::exception& e) {
        logF("[AntiCheatBlocker] Error loading learned anti-cheats: %s", e.what());
    }
}

void AntiCheatBlocker::saveLearnedAntiCheats() {
    std::lock_guard<std::mutex> lock(learnedMutex);
    
    std::wstring filePath = getLearnedDataFilePath();
    if (filePath.empty()) return;
    
    try {
        std::ofstream file(filePath);
        if (file.is_open()) {
            file << "# Xorion AntiCheatBlocker - Learned Anti-Cheats Database\n";
            file << "# This file is auto-generated. New detections are added automatically.\n";
            file << "# Format: one anti-cheat name per line\n";
            file << "# Submit new entries via PR at: https://github.com/X4V1-code/Xorion\n\n";
            
            for (const auto& name : learnedAntiCheats) {
                file << name << "\n";
            }
            file.close();
            learnedDataModified = false;
            logF("[AntiCheatBlocker] Saved %zu learned anti-cheats to file", learnedAntiCheats.size());
        }
    } catch (const std::exception& e) {
        logF("[AntiCheatBlocker] Error saving learned anti-cheats: %s", e.what());
    }
}

std::string AntiCheatBlocker::toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::string AntiCheatBlocker::sanitizeName(const std::string& name) {
    std::string result;
    for (char c : name) {
        if (std::isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_') {
            result += c;
        }
    }
    return result;
}

bool AntiCheatBlocker::isValidAntiCheatName(const std::string& name) {
    // Must be at least 2 characters and not more than 50
    if (name.length() < 2 || name.length() > 50) return false;
    
    // Must contain at least one letter
    bool hasLetter = false;
    for (char c : name) {
        if (std::isalpha(static_cast<unsigned char>(c))) {
            hasLetter = true;
            break;
        }
    }
    return hasLetter;
}

size_t AntiCheatBlocker::getLearnedAntiCheatsCount() const {
    return learnedAntiCheats.size();
}

std::string AntiCheatBlocker::extractPotentialAntiCheatName(const std::string& message) {
    std::string lowerMsg = toLower(message);
    
    // Look for patterns like "[PluginName]", "(PluginName)", "by PluginName"
    std::vector<std::regex> patterns = {
        std::regex("\\[([a-z][a-z0-9_-]{2,20})\\]"),      // [PluginName]
        std::regex("\\(([a-z][a-z0-9_-]{2,20})\\)"),      // (PluginName)
        std::regex("by\\s+([a-z][a-z0-9_-]{2,20})"),      // by PluginName
        std::regex("from\\s+([a-z][a-z0-9_-]{2,20})"),    // from PluginName
        std::regex("detected by\\s+([a-z][a-z0-9_-]{2,20})"),  // detected by PluginName
    };
    
    for (const auto& pattern : patterns) {
        std::smatch match;
        if (std::regex_search(lowerMsg, match, pattern)) {
            std::string potentialName = match[1].str();
            // Don't learn common words
            static const std::set<std::string> commonWords = {
                "the", "and", "for", "you", "are", "was", "has", "have",
                "server", "player", "staff", "admin", "mod", "owner"
            };
            if (commonWords.find(potentialName) == commonWords.end()) {
                return potentialName;
            }
        }
    }
    return "";
}

void AntiCheatBlocker::learnNewAntiCheat(const std::string& name) {
    std::string lowerName = toLower(sanitizeName(name));
    
    if (!isValidAntiCheatName(lowerName)) return;
    
    // Check if already known (built-in)
    for (const auto& builtin : builtInAntiCheats) {
        if (builtin == lowerName) return;
    }
    
    std::lock_guard<std::mutex> lock(learnedMutex);
    
    // Check if already learned
    if (learnedAntiCheats.find(lowerName) != learnedAntiCheats.end()) return;
    
    // Add new anti-cheat
    learnedAntiCheats.insert(lowerName);
    learnedDataModified = true;
    
    logF("[AntiCheatBlocker] LEARNED NEW ANTI-CHEAT: %s", lowerName.c_str());
    
    auto clientInstance = Game.getClientInstance();
    if (clientInstance != nullptr) {
        auto guiData = clientInstance->getGuiData();
        if (guiData != nullptr) {
            guiData->displayClientMessageF("§6[AntiCheatBlocker] §fLearned new anti-cheat: §e%s", lowerName.c_str());
            guiData->displayClientMessageF("§6[AntiCheatBlocker] §fPlease submit to: §bgithub.com/X4V1-code/Xorion");
        }
    }
}

bool AntiCheatBlocker::detectAntiCheatInMessage(const std::string& message) {
    std::string lowerMsg = toLower(message);
    
    // Check built-in anti-cheats
    for (const auto& acName : builtInAntiCheats) {
        if (lowerMsg.find(acName) != std::string::npos) {
            logF("[AntiCheatBlocker] Detected built-in anti-cheat reference: %s", acName.c_str());
            detectionCount++;
            return true;
        }
    }
    
    // Check learned anti-cheats
    {
        std::lock_guard<std::mutex> lock(learnedMutex);
        for (const auto& acName : learnedAntiCheats) {
            if (lowerMsg.find(acName) != std::string::npos) {
                logF("[AntiCheatBlocker] Detected learned anti-cheat reference: %s", acName.c_str());
                detectionCount++;
                return true;
            }
        }
    }
    
    // Try to learn from the message
    std::string potentialName = extractPotentialAntiCheatName(message);
    if (!potentialName.empty() && detectSuspiciousActivity(message)) {
        learnNewAntiCheat(potentialName);
    }
    
    return false;
}

bool AntiCheatBlocker::detectSuspiciousActivity(const std::string& message) {
    std::string lowerMsg = toLower(message);
    
    for (const auto& keyword : suspiciousKeywords) {
        if (lowerMsg.find(keyword) != std::string::npos) {
            suspiciousActivityScore++;
            logF("[AntiCheatBlocker] Suspicious activity detected: %s (score: %d)", keyword.c_str(), suspiciousActivityScore);
            return true;
        }
    }
    return false;
}

bool AntiCheatBlocker::detectBanCommand(const std::string& command) {
    std::string lowerCmd = toLower(command);
    
    for (const auto& banPattern : banCommandPatterns) {
        if (lowerCmd.length() >= banPattern.length() && 
            lowerCmd.substr(0, banPattern.length()) == banPattern) {
            return true;
        }
    }
    return false;
}

void AntiCheatBlocker::triggerAggressiveProtection() {
    aggressiveModeActive = true;
    aggressiveBlockCount++;
    
    auto clientInstance = Game.getClientInstance();
    if (clientInstance == nullptr) return;
    
    auto guiData = clientInstance->getGuiData();
    if (guiData != nullptr) {
        guiData->displayClientMessageF("§4[AntiCheatBlocker] §c§lAGGRESSIVE MODE ACTIVATED!");
        guiData->displayClientMessageF("§4[AntiCheatBlocker] §cBlocking all suspicious packets...");
        guiData->displayClientMessageF("§4[AntiCheatBlocker] §c§lDISCONNECT IMMEDIATELY!");
    }
    
    logF("[AntiCheatBlocker] AGGRESSIVE MODE ACTIVATED - Block count: %d", aggressiveBlockCount);
}

void AntiCheatBlocker::triggerSafeDisconnect() {
    if (hasKickedPlayer) return;
    
    auto clientInstance = Game.getClientInstance();
    if (clientInstance == nullptr) return;
    
    auto guiData = clientInstance->getGuiData();
    if (guiData != nullptr) {
        guiData->displayClientMessageF("§c§l[AntiCheatBlocker] §f§lANTI-CHEAT DETECTED!");
        guiData->displayClientMessageF("§c[AntiCheatBlocker] §fDetection count: §e%d", detectionCount);
    }
    
    logF("[AntiCheatBlocker] Triggering safe disconnect - Detection count: %d", detectionCount);
    
    hasKickedPlayer = true;
    detectionTriggered = true;
    
    // Activate aggressive mode
    if (detectionCount >= 2 || suspiciousActivityScore >= 3) {
        triggerAggressiveProtection();
    }
    
    if (guiData != nullptr) {
        guiData->displayClientMessageF("§c§l[AntiCheatBlocker] §f§lLEAVE THE SERVER NOW!");
        guiData->displayClientMessageF("§c[AntiCheatBlocker] §fPress ESC and click 'Disconnect'!");
        guiData->displayClientMessageF("§c[AntiCheatBlocker] §fAutomatic disconnect not available!");
    }
}

void AntiCheatBlocker::blockAllSuspiciousPackets(Packet* packet) {
    if (!aggressiveModeActive) return;
    
    // In aggressive mode, block/modify suspicious outgoing packets
    if (packet->isInstanceOf<TextPacket>()) {
        TextPacket* textPacket = static_cast<TextPacket*>(packet);
        textPacket->message.setText("");  // Clear the message
        logF("[AntiCheatBlocker] BLOCKED suspicious text packet");
    }
    
    if (packet->isInstanceOf<CommandRequestPacket>()) {
        CommandRequestPacket* cmdPacket = static_cast<CommandRequestPacket*>(packet);
        cmdPacket->command.setText("");  // Clear the command
        logF("[AntiCheatBlocker] BLOCKED suspicious command packet");
    }
}

void AntiCheatBlocker::interceptBanCommand(Packet* packet) {
    if (packet->isInstanceOf<CommandRequestPacket>()) {
        CommandRequestPacket* cmdPacket = static_cast<CommandRequestPacket*>(packet);
        std::string cmd = cmdPacket->command.getText();
        
        if (detectBanCommand(cmd)) {
            logF("[AntiCheatBlocker] INTERCEPTED ban command: %s", cmd.c_str());
            cmdPacket->command.setText("");  // Block the command
            
            auto clientInstance = Game.getClientInstance();
            if (clientInstance != nullptr) {
                auto guiData = clientInstance->getGuiData();
                if (guiData != nullptr) {
                    guiData->displayClientMessageF("§a§l[AntiCheatBlocker] §f§lBLOCKED ban command!");
                    guiData->displayClientMessageF("§a[AntiCheatBlocker] §fCommand: §7%s", cmd.c_str());
                }
            }
            
            // Activate aggressive protection
            triggerAggressiveProtection();
        }
    }
}

void AntiCheatBlocker::onTick(GameMode* gm) {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastCheckTime);
    
    // Check every 2 seconds (more aggressive)
    if (elapsed.count() >= 2) {
        lastCheckTime = now;
        
        // Reset detection flags if player is no longer in game
        if (!Game.isInGame()) {
            hasKickedPlayer = false;
            detectionTriggered = false;
            aggressiveModeActive = false;
            suspiciousActivityScore = 0;
            detectionCount = 0;
        }
        
        // Save learned anti-cheats periodically (every 30 seconds if modified)
        auto saveElapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastSaveTime);
        if (saveElapsed.count() >= 30 && learnedDataModified) {
            saveLearnedAntiCheats();
            lastSaveTime = now;
        }
    }
}

void AntiCheatBlocker::onSendPacket(Packet* packet) {
    if (packet == nullptr) return;
    
    // If aggressive mode is active, block all suspicious packets
    if (aggressiveModeActive) {
        blockAllSuspiciousPackets(packet);
        return;
    }
    
    // Monitor outgoing chat messages
    if (packet->isInstanceOf<TextPacket>()) {
        TextPacket* textPacket = static_cast<TextPacket*>(packet);
        std::string message = textPacket->message.getText();
        
        // Check for anti-cheat references
        if (detectAntiCheatInMessage(message)) {
            triggerSafeDisconnect();
        }
        
        // Check for suspicious activity
        if (detectSuspiciousActivity(message)) {
            if (suspiciousActivityScore >= 3) {
                triggerSafeDisconnect();
            }
        }
    }
    
    // Intercept command packets to block ban commands
    if (packet->isInstanceOf<CommandRequestPacket>()) {
        interceptBanCommand(packet);
    }
}
