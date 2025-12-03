#include "AntiCheatDelete.h"
#include "AntiCheatBlocker.h"
#include "../ModuleManager.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/Packet.h"
#include "../../../SDK/TextHolder.h"
#include "../../../Utils/Logger.h"
#include <algorithm>
#include <cctype>

// Server-wide deletion commands - these try to uninstall/disable anti-cheat for everyone
const std::vector<std::string> AntiCheatDelete::serverDeleteCommands = {
    // Plugin management commands (require OP/admin)
    "/plugins disable %s",
    "/pl disable %s",
    "/plugman disable %s",
    "/plugman unload %s",
    "/plugins unload %s",
    "/pl unload %s",
    "/plugin unload %s",
    "/plugin remove %s",
    "/plugins remove %s",
    "/plugman remove %s",
    // Direct management
    "/stop %s",
    "/reload %s",
    "/rl %s",
    // Permission removal
    "/lp group default permission unset %s.*",
    "/perm group default remove %s.*",
    "/permissions remove %s.*",
    // Disable specific features
    "/%s disable",
    "/%s stop",
    "/%s off",
    "/%s toggle off",
    "/%s bypass on",
    "/%s exempt *",
    // Config manipulation
    "/%s config bypass true",
    "/%s config enabled false",
    "/%s set enabled false",
    // Force stop
    "/minecraft:kill @e[type=%s]",
    "/execute as @a run %s bypass",
    // Operator bypass
    "/op @s",
    "/deop %s",
};

// Player-only deletion commands - these try to exempt/bypass just the current player
const std::vector<std::string> AntiCheatDelete::playerDeleteCommands = {
    // Self exemption commands
    "/%s exempt add @s",
    "/%s exempt @s",
    "/%s bypass @s",
    "/%s bypass add @s",
    "/%s whitelist @s",
    "/%s whitelist add @s",
    "/%s ignore @s",
    "/%s ignore add @s",
    "/%s trust @s",
    "/%s trust add @s",
    // Permission bypass
    "/lp user @s permission set %s.bypass true",
    "/perm user @s add %s.bypass",
    "/permissions add @s %s.bypass",
    // Toggle for self
    "/%s toggle @s off",
    "/%s disable @s",
    // Specific anti-cheat exemptions
    "/%s exempt @s fly",
    "/%s exempt @s speed",
    "/%s exempt @s killaura",
    "/%s exempt @s reach",
    "/%s exempt @s scaffold",
    "/%s exempt @s all",
    // Generic bypass
    "/%s bypass all @s",
    "/%s bypass mode creative @s",
};

// Known anti-cheat plugin names to try commands against
const std::vector<std::string> AntiCheatDelete::antiCheatPluginNames = {
    // Major Anti-Cheats
    "anticheat", "nocheatplus", "ncp", "aac", "advancedanticheat",
    "spartan", "matrix", "vulcan", "verus", "grim", "grimac",
    "karhu", "intave", "polar", "watchdog", "sentinel",
    // Screen Share Tools
    "screenshare", "paladin",
    // Other Anti-Cheats
    "reflex", "horizon", "themis", "wraith", "badlion",
    "hac", "negativity", "kauri", "godseye", "hawk",
    "angel", "alice", "artemis", "sonar", "arc",
    "guardian", "cerberus", "aegis", "warden", "hydra",
    "phantom", "specter", "overwatch"
};

AntiCheatDelete::AntiCheatDelete()
    : IModule(0, Category::MISC, "Aggressively attempts to delete/disable anti-cheat. Server-wide first, then player-only, then falls back to blocker.") {
    lastAttemptTime = std::chrono::steady_clock::now();
    startTime = std::chrono::steady_clock::now();
    
    logF("[AntiCheatDelete] Initialized - Will try server-wide deletion first");
}

AntiCheatDelete::~AntiCheatDelete() {
}

const char* AntiCheatDelete::getModuleName() {
    return "AntiCheatDelete";
}

void AntiCheatDelete::onEnable() {
    // Reset state when enabled
    currentMode = DeletionMode::SERVER_WIDE;
    attemptCount = 0;
    currentCommandIndex = 0;
    deletionSuccessful = false;
    hasTriggeredBlocker = false;
    aggressiveRetryActive = true;
    aggressiveRetryCount = 0;
    startTime = std::chrono::steady_clock::now();
    
    auto clientInstance = Game.getClientInstance();
    if (clientInstance != nullptr) {
        auto guiData = clientInstance->getGuiData();
        if (guiData != nullptr) {
            guiData->displayClientMessageF("§6[AntiCheatDelete] §a§lSTARTED!");
            guiData->displayClientMessageF("§6[AntiCheatDelete] §fAttempting server-wide anti-cheat deletion...");
            guiData->displayClientMessageF("§6[AntiCheatDelete] §7This will try aggressively for up to 2 minutes.");
        }
    }
    
    logF("[AntiCheatDelete] Enabled - Starting aggressive anti-cheat deletion");
}

void AntiCheatDelete::onDisable() {
    aggressiveRetryActive = false;
    
    auto clientInstance = Game.getClientInstance();
    if (clientInstance != nullptr) {
        auto guiData = clientInstance->getGuiData();
        if (guiData != nullptr) {
            guiData->displayClientMessageF("§6[AntiCheatDelete] §7Disabled");
        }
    }
    
    logF("[AntiCheatDelete] Disabled");
}

std::string AntiCheatDelete::toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::string AntiCheatDelete::replacePluginName(const std::string& cmdTemplate, const std::string& pluginName) {
    std::string cmd = cmdTemplate;
    size_t pos = cmd.find("%s");
    while (pos != std::string::npos) {
        cmd.replace(pos, 2, pluginName);
        pos = cmd.find("%s", pos + pluginName.length());
    }
    return cmd;
}

void AntiCheatDelete::sendCommand(const std::string& command) {
    auto clientInstance = Game.getClientInstance();
    if (clientInstance == nullptr || clientInstance->loopbackPacketSender == nullptr) {
        return;
    }
    
    // Create and send command packet
    CommandRequestPacket cmdPacket;
    cmdPacket.command.setText(command);
    cmdPacket.origin = CommandRequestPacket::Origin::PLAYER;
    clientInstance->loopbackPacketSender->sendToServer(&cmdPacket);
    
    logF("[AntiCheatDelete] Sent command: %s", command.c_str());
}

void AntiCheatDelete::sendMultipleCommands(const std::vector<std::string>& commands) {
    for (const auto& cmd : commands) {
        sendCommand(cmd);
    }
}

std::vector<std::string> AntiCheatDelete::generateServerDeleteCommands(const std::string& pluginName) {
    std::vector<std::string> commands;
    
    for (const auto& cmdTemplate : serverDeleteCommands) {
        commands.push_back(replacePluginName(cmdTemplate, pluginName));
    }
    
    return commands;
}

std::vector<std::string> AntiCheatDelete::generatePlayerDeleteCommands(const std::string& pluginName) {
    std::vector<std::string> commands;
    
    for (const auto& cmdTemplate : playerDeleteCommands) {
        commands.push_back(replacePluginName(cmdTemplate, pluginName));
    }
    
    return commands;
}

void AntiCheatDelete::attemptServerWideDeletion() {
    if (!aggressiveRetryActive || deletionSuccessful) return;
    
    std::lock_guard<std::mutex> lock(stateMutex);
    
    // Calculate which plugin and command to try
    size_t totalCombinations = antiCheatPluginNames.size() * serverDeleteCommands.size();
    
    if (attemptCount >= maxServerAttempts || currentCommandIndex >= totalCombinations) {
        // Exhausted server-wide attempts, transition to player-only mode
        transitionToPlayerMode();
        return;
    }
    
    // Calculate current plugin and command
    size_t pluginIndex = currentCommandIndex / serverDeleteCommands.size();
    size_t cmdIndex = currentCommandIndex % serverDeleteCommands.size();
    
    if (pluginIndex >= antiCheatPluginNames.size()) {
        transitionToPlayerMode();
        return;
    }
    
    const std::string& pluginName = antiCheatPluginNames[pluginIndex];
    const std::string& cmdTemplate = serverDeleteCommands[cmdIndex];
    
    // Generate and send command using helper function
    std::string cmd = replacePluginName(cmdTemplate, pluginName);
    sendCommand(cmd);
    
    attemptCount++;
    currentCommandIndex++;
    aggressiveRetryCount++;
    
    // Status update every 10 attempts
    if (attemptCount % 10 == 0) {
        auto clientInstance = Game.getClientInstance();
        if (clientInstance != nullptr) {
            auto guiData = clientInstance->getGuiData();
            if (guiData != nullptr) {
                guiData->displayClientMessageF("§6[AntiCheatDelete] §eServer deletion attempts: %d/%d", 
                    attemptCount, maxServerAttempts);
            }
        }
    }
}

void AntiCheatDelete::attemptPlayerOnlyDeletion() {
    if (!aggressiveRetryActive || deletionSuccessful) return;
    
    std::lock_guard<std::mutex> lock(stateMutex);
    
    // Calculate which plugin and command to try
    size_t totalCombinations = antiCheatPluginNames.size() * playerDeleteCommands.size();
    size_t playerCommandIndex = currentCommandIndex - (antiCheatPluginNames.size() * serverDeleteCommands.size());
    
    if (attemptCount >= (maxServerAttempts + maxPlayerAttempts) || playerCommandIndex >= totalCombinations) {
        // Exhausted player-only attempts, fall back to blocker
        transitionToBlockerMode();
        return;
    }
    
    // Calculate current plugin and command
    size_t pluginIndex = playerCommandIndex / playerDeleteCommands.size();
    size_t cmdIndex = playerCommandIndex % playerDeleteCommands.size();
    
    if (pluginIndex >= antiCheatPluginNames.size()) {
        transitionToBlockerMode();
        return;
    }
    
    const std::string& pluginName = antiCheatPluginNames[pluginIndex];
    const std::string& cmdTemplate = playerDeleteCommands[cmdIndex];
    
    // Generate and send command using helper function
    std::string cmd = replacePluginName(cmdTemplate, pluginName);
    sendCommand(cmd);
    
    attemptCount++;
    currentCommandIndex++;
    aggressiveRetryCount++;
    
    // Status update every 10 attempts
    if (attemptCount % 10 == 0) {
        auto clientInstance = Game.getClientInstance();
        if (clientInstance != nullptr) {
            auto guiData = clientInstance->getGuiData();
            if (guiData != nullptr) {
                guiData->displayClientMessageF("§6[AntiCheatDelete] §ePlayer-only deletion attempts: %d/%d", 
                    attemptCount - maxServerAttempts, maxPlayerAttempts);
            }
        }
    }
}

void AntiCheatDelete::transitionToPlayerMode() {
    currentMode = DeletionMode::PLAYER_ONLY;
    
    auto clientInstance = Game.getClientInstance();
    if (clientInstance != nullptr) {
        auto guiData = clientInstance->getGuiData();
        if (guiData != nullptr) {
            guiData->displayClientMessageF("§6[AntiCheatDelete] §c§lSERVER-WIDE DELETION FAILED!");
            guiData->displayClientMessageF("§6[AntiCheatDelete] §eSwitching to player-only bypass mode...");
        }
    }
    
    logF("[AntiCheatDelete] Server-wide deletion failed after %d attempts. Switching to player-only mode.", attemptCount);
}

void AntiCheatDelete::transitionToBlockerMode() {
    currentMode = DeletionMode::BLOCKER_FALLBACK;
    aggressiveRetryActive = false;
    
    auto clientInstance = Game.getClientInstance();
    if (clientInstance != nullptr) {
        auto guiData = clientInstance->getGuiData();
        if (guiData != nullptr) {
            guiData->displayClientMessageF("§6[AntiCheatDelete] §c§lALL DELETION ATTEMPTS FAILED!");
            guiData->displayClientMessageF("§6[AntiCheatDelete] §fFalling back to AntiCheatBlocker...");
        }
    }
    
    logF("[AntiCheatDelete] All deletion attempts failed after %d total attempts. Falling back to AntiCheatBlocker.", attemptCount);
    
    fallbackToBlocker();
}

AntiCheatBlocker* AntiCheatDelete::getAntiCheatBlocker() {
    if (moduleMgr != nullptr) {
        return moduleMgr->getModule<AntiCheatBlocker>();
    }
    return nullptr;
}

void AntiCheatDelete::fallbackToBlocker() {
    if (hasTriggeredBlocker) return;
    hasTriggeredBlocker = true;
    
    // Ensure AntiCheatBlocker is enabled
    AntiCheatBlocker* blocker = getAntiCheatBlocker();
    if (blocker != nullptr) {
        if (!blocker->isEnabled()) {
            blocker->setEnabled(true);
        }
        
        auto clientInstance = Game.getClientInstance();
        if (clientInstance != nullptr) {
            auto guiData = clientInstance->getGuiData();
            if (guiData != nullptr) {
                guiData->displayClientMessageF("§a[AntiCheatDelete] §fAntiCheatBlocker is now active!");
                guiData->displayClientMessageF("§a[AntiCheatDelete] §7Blocking anti-cheat packets passively...");
            }
        }
        
        logF("[AntiCheatDelete] Successfully activated AntiCheatBlocker fallback");
    } else {
        logF("[AntiCheatDelete] WARNING: Could not find AntiCheatBlocker module!");
    }
}

bool AntiCheatDelete::detectDeletionSuccess(const std::string& message) {
    std::string lowerMsg = toLower(message);
    
    // Success indicators
    static const std::vector<std::string> successPatterns = {
        "plugin disabled", "successfully disabled", "has been disabled",
        "plugin unloaded", "successfully unloaded", "has been unloaded",
        "plugin removed", "successfully removed", "has been removed",
        "bypass enabled", "exemption added", "whitelisted",
        "permission granted", "you are now exempt", "bypass activated"
    };
    
    for (const auto& pattern : successPatterns) {
        if (lowerMsg.find(pattern) != std::string::npos) {
            return true;
        }
    }
    
    return false;
}

bool AntiCheatDelete::detectDeletionFailure(const std::string& message) {
    std::string lowerMsg = toLower(message);
    
    // Failure indicators
    static const std::vector<std::string> failurePatterns = {
        "unknown command", "permission denied", "no permission",
        "not allowed", "access denied", "forbidden",
        "plugin not found", "unknown plugin", "does not exist",
        "cannot disable", "cannot unload", "error", "failed"
    };
    
    for (const auto& pattern : failurePatterns) {
        if (lowerMsg.find(pattern) != std::string::npos) {
            return true;
        }
    }
    
    return false;
}

void AntiCheatDelete::handleResponse(const std::string& message) {
    if (detectDeletionSuccess(message)) {
        deletionSuccessful = true;
        aggressiveRetryActive = false;
        
        auto clientInstance = Game.getClientInstance();
        if (clientInstance != nullptr) {
            auto guiData = clientInstance->getGuiData();
            if (guiData != nullptr) {
                guiData->displayClientMessageF("§a§l[AntiCheatDelete] SUCCESS!");
                guiData->displayClientMessageF("§a[AntiCheatDelete] §fAnti-cheat has been disabled/bypassed!");
            }
        }
        
        logF("[AntiCheatDelete] SUCCESS - Anti-cheat deletion/bypass confirmed!");
    }
}

void AntiCheatDelete::onTick(GameMode* gm) {
    if (!Game.isInGame()) {
        // Reset state if not in game
        if (currentMode != DeletionMode::SERVER_WIDE) {
            currentMode = DeletionMode::SERVER_WIDE;
            attemptCount = 0;
            currentCommandIndex = 0;
            hasTriggeredBlocker = false;
        }
        return;
    }
    
    // Check if we've been trying too long
    auto now = std::chrono::steady_clock::now();
    auto totalDuration = std::chrono::duration_cast<std::chrono::seconds>(now - startTime);
    
    if (totalDuration.count() >= maxRetryDurationSeconds && aggressiveRetryActive) {
        // Time's up - fall back to blocker if we haven't already
        if (currentMode != DeletionMode::BLOCKER_FALLBACK) {
            transitionToBlockerMode();
        }
        return;
    }
    
    // If deletion was successful or blocker is active, nothing more to do
    if (deletionSuccessful || currentMode == DeletionMode::BLOCKER_FALLBACK) {
        return;
    }
    
    // Check if enough time has passed since last attempt
    auto attemptElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastAttemptTime);
    
    if (attemptElapsed.count() < attemptIntervalMs) {
        return;
    }
    
    lastAttemptTime = now;
    
    // Check aggressive retry limit
    if (aggressiveRetryCount >= maxAggressiveRetries) {
        if (currentMode != DeletionMode::BLOCKER_FALLBACK) {
            transitionToBlockerMode();
        }
        return;
    }
    
    // Perform deletion attempt based on current mode
    switch (currentMode) {
        case DeletionMode::SERVER_WIDE:
            attemptServerWideDeletion();
            break;
        case DeletionMode::PLAYER_ONLY:
            attemptPlayerOnlyDeletion();
            break;
        case DeletionMode::BLOCKER_FALLBACK:
            // Nothing to do - blocker handles it
            break;
    }
}

void AntiCheatDelete::onSendPacket(Packet* packet) {
    if (packet == nullptr) return;
    
    // Monitor text packets for server responses
    // TextPackets can contain echoed server messages/responses
    if (packet->isInstanceOf<TextPacket>()) {
        TextPacket* textPacket = static_cast<TextPacket*>(packet);
        std::string message = textPacket->message.getText();
        
        // Check for success/failure indicators in responses
        if (!message.empty()) {
            handleResponse(message);
        }
    }
}
