#include "Unbanner.h"
#include "../../DrawUtils.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/Packet.h"
#include "../../../SDK/TextHolder.h"
#include "../../../Utils/Utils.h"
#include "../../../Utils/Logger.h"
#include <random>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>

// Xorion green color (same as xorion-banner.png)
static const MC_Color xorionGreen = MC_Color(37, 164, 64);
static const MC_Color whiteColor = MC_Color(255, 255, 255);
static const MC_Color redColor = MC_Color(255, 80, 80);
static const MC_Color orangeColor = MC_Color(255, 165, 0);

// Ban/kick command patterns to block
const std::vector<std::string> Unbanner::banKickPatterns = {
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
    // Server moderation
    "/execute ", "/sudo ",
    "/kill ", "/slay ", "/smite ",
    // Network bans
    "/gban ", "/globalban ", "/netban ", "/networkban "
};

// Word list for generating random usernames
const char* Unbanner::wordList[] = {
    "Player", "Gamer", "Hero", "Knight", "Dragon", "Shadow", "Storm", "Fire",
    "Ice", "Thunder", "Wolf", "Eagle", "Tiger", "Lion", "Bear", "Hawk",
    "Ninja", "Warrior", "Mage", "Ranger", "Hunter", "Scout", "Blade", "Sword",
    "Shield", "Arrow", "Spark", "Blaze", "Frost", "Wind", "Stone", "Iron",
    "Steel", "Gold", "Silver", "Bronze", "Copper", "Ruby", "Emerald", "Diamond",
    "Swift", "Quick", "Fast", "Rapid", "Sonic", "Flash", "Bolt", "Strike",
    "Mighty", "Strong", "Power", "Force", "Energy", "Spirit", "Soul", "Heart",
    "Dark", "Light", "Bright", "Glow", "Shine", "Beam", "Ray", "Star"
};
const int Unbanner::wordListSize = sizeof(wordList) / sizeof(wordList[0]);

Unbanner::Unbanner()
    : IModule(0, Category::MISC, "Aggressively blocks ban/kick commands and spoofs identity. Blocks chat during protection.") {
    registerBoolSetting("SpoofName", &spoofName, spoofName);
    registerBoolSetting("SpoofDeviceId", &spoofDeviceId, spoofDeviceId);
    registerBoolSetting("SpoofXuid", &spoofXuid, spoofXuid);
    registerBoolSetting("SpoofIP", &spoofIP, spoofIP);
    registerBoolSetting("ShowButton", &showButton, showButton);
    
    // Initialize defense state
    currentDefenseLevel = DefenseLevel::NORMAL;
    chatBlocked = false;
    blockedCommandCount = 0;
    blockingFailureCount = 0;
}

Unbanner::~Unbanner() {
    // Clean up the fake name holder if it exists
    if (fakeNameHolder != nullptr) {
        if (Game.getFakeName() == fakeNameHolder) {
            Game.setFakeName(nullptr);
        }
        delete fakeNameHolder;
        fakeNameHolder = nullptr;
    }
    
    // Clean up Device ID holder
    if (fakeDeviceIdHolder != nullptr) {
        if (Game.getFakeDeviceId() == fakeDeviceIdHolder) {
            Game.setFakeDeviceId(nullptr);
        }
        delete fakeDeviceIdHolder;
        fakeDeviceIdHolder = nullptr;
    }
    
    // Clean up XUID holder
    if (fakeXuidHolder != nullptr) {
        if (Game.getFakeXuid() == fakeXuidHolder) {
            Game.setFakeXuid(nullptr);
        }
        delete fakeXuidHolder;
        fakeXuidHolder = nullptr;
    }
    
    // Clean up IP holder
    if (fakeIPHolder != nullptr) {
        if (Game.getFakeIP() == fakeIPHolder) {
            Game.setFakeIP(nullptr);
        }
        delete fakeIPHolder;
        fakeIPHolder = nullptr;
    }
}

const char* Unbanner::getModuleName() {
    return "Unbanner";
}

void Unbanner::generateSpoofedUsername() {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Pick a random word from the list
    std::uniform_int_distribution<> wordDist(0, wordListSize - 1);
    std::string word = wordList[wordDist(gen)];
    
    // Generate 3 random digits
    std::uniform_int_distribution<> digitDist(0, 9);
    std::string numbers;
    for (int i = 0; i < 3; i++) {
        numbers += std::to_string(digitDist(gen));
    }
    
    spoofedUsername = word + numbers;
    usernameGenerated = true;
    
    // Clean up old holder before creating new one
    if (fakeNameHolder != nullptr) {
        if (Game.getFakeName() == fakeNameHolder) {
            Game.setFakeName(nullptr);
        }
        delete fakeNameHolder;
        fakeNameHolder = nullptr;
    }
    fakeNameHolder = new TextHolder(spoofedUsername);
    Game.setFakeName(fakeNameHolder);
}

void Unbanner::generateSpoofedDeviceId() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> hexDist(0, 15);
    
    // Generate UUID format: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
    std::stringstream ss;
    const char* hexChars = "0123456789abcdef";
    
    // First 8 chars
    for (int i = 0; i < 8; i++) ss << hexChars[hexDist(gen)];
    ss << "-";
    // Next 4 chars
    for (int i = 0; i < 4; i++) ss << hexChars[hexDist(gen)];
    ss << "-";
    // Next 4 chars
    for (int i = 0; i < 4; i++) ss << hexChars[hexDist(gen)];
    ss << "-";
    // Next 4 chars
    for (int i = 0; i < 4; i++) ss << hexChars[hexDist(gen)];
    ss << "-";
    // Last 12 chars
    for (int i = 0; i < 12; i++) ss << hexChars[hexDist(gen)];
    
    spoofedDeviceId = ss.str();
    deviceIdGenerated = true;
    
    // Clean up old holder before creating new one
    if (fakeDeviceIdHolder != nullptr) {
        if (Game.getFakeDeviceId() == fakeDeviceIdHolder) {
            Game.setFakeDeviceId(nullptr);
        }
        delete fakeDeviceIdHolder;
        fakeDeviceIdHolder = nullptr;
    }
    fakeDeviceIdHolder = new TextHolder(spoofedDeviceId);
    Game.setFakeDeviceId(fakeDeviceIdHolder);
}

void Unbanner::generateSpoofedXuid() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    
    // XUID is typically a 16-digit number (range 1-2 quadrillion)
    std::uniform_int_distribution<uint64_t> dist(1000000000000000ULL, 2999999999999999ULL);
    uint64_t xuidValue = dist(gen);
    
    spoofedXuid = std::to_string(xuidValue);
    xuidGenerated = true;
    
    // Clean up old holder before creating new one
    if (fakeXuidHolder != nullptr) {
        if (Game.getFakeXuid() == fakeXuidHolder) {
            Game.setFakeXuid(nullptr);
        }
        delete fakeXuidHolder;
        fakeXuidHolder = nullptr;
    }
    fakeXuidHolder = new TextHolder(spoofedXuid);
    Game.setFakeXuid(fakeXuidHolder);
}

void Unbanner::generateSpoofedIP() {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Generate a realistic-looking public IP address
    // Avoid private IP ranges (10.x.x.x, 172.16-31.x.x, 192.168.x.x)
    // and reserved ranges (0.x.x.x, 127.x.x.x, 224-255.x.x.x)
    std::uniform_int_distribution<> firstOctetDist(1, 223);
    std::uniform_int_distribution<> octetDist(0, 255);
    
    int firstOctet, secondOctet, thirdOctet, fourthOctet;
    bool isValid;
    
    do {
        firstOctet = firstOctetDist(gen);
        secondOctet = octetDist(gen);
        thirdOctet = octetDist(gen);
        fourthOctet = octetDist(gen);
        
        isValid = true;
        
        // Skip private and reserved ranges
        if (firstOctet == 10) isValid = false;                           // 10.x.x.x private
        else if (firstOctet == 127) isValid = false;                     // 127.x.x.x loopback
        else if (firstOctet == 172 && secondOctet >= 16 && secondOctet <= 31) isValid = false;  // 172.16-31.x.x private
        else if (firstOctet == 192 && secondOctet == 168) isValid = false;  // 192.168.x.x private
        else if (firstOctet == 0) isValid = false;                       // 0.x.x.x reserved
        else if (firstOctet >= 224) isValid = false;                     // 224-255.x.x.x multicast/reserved
        
    } while (!isValid);
    
    // Format as IPv4 address string
    std::stringstream ss;
    ss << firstOctet << "." << secondOctet << "." << thirdOctet << "." << fourthOctet;
    
    spoofedIP = ss.str();
    ipGenerated = true;
    
    // Clean up old holder before creating new one
    if (fakeIPHolder != nullptr) {
        if (Game.getFakeIP() == fakeIPHolder) {
            Game.setFakeIP(nullptr);
        }
        delete fakeIPHolder;
        fakeIPHolder = nullptr;
    }
    fakeIPHolder = new TextHolder(spoofedIP);
    Game.setFakeIP(fakeIPHolder);
}

void Unbanner::generateAllSpoofedIds() {
    if (spoofName) generateSpoofedUsername();
    if (spoofDeviceId) generateSpoofedDeviceId();
    if (spoofXuid) generateSpoofedXuid();
    if (spoofIP) generateSpoofedIP();
    
    // Log the generated values
    auto clientInstance = Game.getClientInstance();
    if (clientInstance != nullptr) {
        auto guiData = clientInstance->getGuiData();
        if (guiData != nullptr) {
            if (spoofName && usernameGenerated) {
                guiData->displayClientMessageF("[Unbanner] Username: %s", spoofedUsername.c_str());
            }
            if (spoofDeviceId && deviceIdGenerated) {
                guiData->displayClientMessageF("[Unbanner] Device ID: %s", spoofedDeviceId.c_str());
            }
            if (spoofXuid && xuidGenerated) {
                guiData->displayClientMessageF("[Unbanner] XUID: %s", spoofedXuid.c_str());
            }
            if (spoofIP && ipGenerated) {
                guiData->displayClientMessageF("[Unbanner] VPN IP: %s", spoofedIP.c_str());
            }
        }
    }
}

const std::string& Unbanner::getSpoofedUsername() const {
    return spoofedUsername;
}

const std::string& Unbanner::getSpoofedDeviceId() const {
    return spoofedDeviceId;
}

const std::string& Unbanner::getSpoofedXuid() const {
    return spoofedXuid;
}

const std::string& Unbanner::getSpoofedIP() const {
    return spoofedIP;
}

// Helper function to convert string to lowercase
static std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

bool Unbanner::detectBanKickCommand(const std::string& command) {
    std::string lowerCmd = toLower(command);
    
    for (const auto& pattern : banKickPatterns) {
        if (lowerCmd.length() >= pattern.length() && 
            lowerCmd.substr(0, pattern.length()) == pattern) {
            return true;
        }
    }
    return false;
}

bool Unbanner::blockBanKickCommand(Packet* packet) {
    if (!packet->isInstanceOf<CommandRequestPacket>()) {
        return false;
    }
    
    CommandRequestPacket* cmdPacket = static_cast<CommandRequestPacket*>(packet);
    std::string cmd = cmdPacket->command.getText();
    
    if (detectBanKickCommand(cmd)) {
        // Block the command by clearing it
        cmdPacket->command.setText("");
        blockedCommandCount++;
        
        logF("[Unbanner] BLOCKED ban/kick command: %s (total blocked: %d)", cmd.c_str(), blockedCommandCount);
        
        auto clientInstance = Game.getClientInstance();
        if (clientInstance != nullptr) {
            auto guiData = clientInstance->getGuiData();
            if (guiData != nullptr) {
                guiData->displayClientMessageF("§a§l[Unbanner] §f§lBLOCKED: §7%s", cmd.c_str());
            }
        }
        
        return true;
    }
    return false;
}

void Unbanner::activateDefense() {
    if (currentDefenseLevel == DefenseLevel::NORMAL) {
        currentDefenseLevel = DefenseLevel::BLOCKING_COMMANDS;
        defenseActivatedTime = std::chrono::steady_clock::now();
        blockChat();
        
        logF("[Unbanner] Defense activated - Aggressively blocking commands");
        
        auto clientInstance = Game.getClientInstance();
        if (clientInstance != nullptr) {
            auto guiData = clientInstance->getGuiData();
            if (guiData != nullptr) {
                guiData->displayClientMessageF("§c§l[Unbanner] §fDefense activated - Blocking ban/kick commands!");
            }
        }
    }
}

void Unbanner::escalateDefense() {
    auto clientInstance = Game.getClientInstance();
    auto guiData = (clientInstance != nullptr) ? clientInstance->getGuiData() : nullptr;
    
    switch (currentDefenseLevel) {
        case DefenseLevel::NORMAL:
            // Activate defense first
            activateDefense();
            break;
            
        case DefenseLevel::BLOCKING_COMMANDS:
            // Blocking failed, escalate to username spoofing
            currentDefenseLevel = DefenseLevel::USERNAME_SPOOFING;
            blockingFailureCount++;
            
            // Generate new spoofed identity immediately
            generateSpoofedUsername();
            
            logF("[Unbanner] Escalating to USERNAME_SPOOFING - Failure count: %d", blockingFailureCount);
            
            if (guiData != nullptr) {
                guiData->displayClientMessageF("§6§l[Unbanner] §fBlocking failed - Spoofing username!");
                guiData->displayClientMessageF("§6[Unbanner] §fNew identity: §e%s", spoofedUsername.c_str());
            }
            break;
            
        case DefenseLevel::USERNAME_SPOOFING:
            // Username spoofing not working, check if we should go to last resort
            blockingFailureCount++;
            
            if (blockingFailureCount >= SPOOF_FAILURE_THRESHOLD) {
                // Last resort - kick user to prevent ban
                currentDefenseLevel = DefenseLevel::LAST_RESORT_KICK;
                
                logF("[Unbanner] LAST RESORT - Preparing to disconnect player");
                
                if (guiData != nullptr) {
                    guiData->displayClientMessageF("§4§l[Unbanner] §f§lLAST RESORT - DISCONNECT NOW!");
                    guiData->displayClientMessageF("§4[Unbanner] §fPress ESC and disconnect immediately!");
                }
            } else {
                // Try regenerating identity
                generateSpoofedUsername();
                generateSpoofedDeviceId();
                generateSpoofedXuid();
                generateSpoofedIP();
                
                if (guiData != nullptr) {
                    guiData->displayClientMessageF("§6[Unbanner] §fRegenerated all identities - Attempt %d/%d", 
                        blockingFailureCount, SPOOF_FAILURE_THRESHOLD);
                }
            }
            break;
            
        case DefenseLevel::LAST_RESORT_KICK:
            // Already at last resort - nothing more to do
            if (guiData != nullptr) {
                guiData->displayClientMessageF("§4§l[Unbanner] §f§lDISCONNECT IMMEDIATELY!");
            }
            break;
    }
}

void Unbanner::blockChat() {
    if (!chatBlocked) {
        chatBlocked = true;
        chatBlockedTime = std::chrono::steady_clock::now();
        
        logF("[Unbanner] Chat blocked for %d seconds", CHAT_BLOCK_DURATION_SECONDS);
        
        auto clientInstance = Game.getClientInstance();
        if (clientInstance != nullptr) {
            auto guiData = clientInstance->getGuiData();
            if (guiData != nullptr) {
                guiData->displayClientMessageF("§e[Unbanner] §fChat temporarily blocked for your protection.");
            }
        }
    }
}

void Unbanner::unblockChat() {
    if (chatBlocked) {
        chatBlocked = false;
        
        logF("[Unbanner] Chat unblocked");
        
        auto clientInstance = Game.getClientInstance();
        if (clientInstance != nullptr) {
            auto guiData = clientInstance->getGuiData();
            if (guiData != nullptr) {
                guiData->displayClientMessageF("§a[Unbanner] §fChat restored.");
            }
        }
    }
}

void Unbanner::resetDefenseState() {
    currentDefenseLevel = DefenseLevel::NORMAL;
    chatBlocked = false;
    blockedCommandCount = 0;
    blockingFailureCount = 0;
    
    logF("[Unbanner] Defense state reset");
}

void Unbanner::onEnable() {
    generateAllSpoofedIds();
    resetDefenseState();
    
    auto clientInstance = Game.getClientInstance();
    if (clientInstance != nullptr) {
        auto guiData = clientInstance->getGuiData();
        if (guiData != nullptr) {
            guiData->displayClientMessageF("[Unbanner] Enabled - Aggressive protection active.");
            guiData->displayClientMessageF("[Unbanner] Will block ban/kick commands and spoof identity.");
        }
    }
}

void Unbanner::onDisable() {
    // Reset defense state
    resetDefenseState();
    
    // Clear all fake IDs from GameData
    Game.setFakeName(nullptr);
    Game.setFakeDeviceId(nullptr);
    Game.setFakeXuid(nullptr);
    Game.setFakeIP(nullptr);
    
    if (fakeNameHolder != nullptr) {
        delete fakeNameHolder;
        fakeNameHolder = nullptr;
    }
    if (fakeDeviceIdHolder != nullptr) {
        delete fakeDeviceIdHolder;
        fakeDeviceIdHolder = nullptr;
    }
    if (fakeXuidHolder != nullptr) {
        delete fakeXuidHolder;
        fakeXuidHolder = nullptr;
    }
    if (fakeIPHolder != nullptr) {
        delete fakeIPHolder;
        fakeIPHolder = nullptr;
    }
    
    usernameGenerated = false;
    deviceIdGenerated = false;
    xuidGenerated = false;
    ipGenerated = false;
    spoofedUsername.clear();
    spoofedDeviceId.clear();
    spoofedXuid.clear();
    spoofedIP.clear();
    
    auto clientInstance = Game.getClientInstance();
    if (clientInstance != nullptr) {
        auto guiData = clientInstance->getGuiData();
        if (guiData != nullptr) {
            guiData->displayClientMessageF("[Unbanner] Disabled - Protection deactivated.");
        }
    }
}

void Unbanner::onTick(GameMode* gm) {
    // Ensure we have spoofed IDs generated
    if (spoofName && !usernameGenerated) {
        generateSpoofedUsername();
    }
    if (spoofDeviceId && !deviceIdGenerated) {
        generateSpoofedDeviceId();
    }
    if (spoofXuid && !xuidGenerated) {
        generateSpoofedXuid();
    }
    if (spoofIP && !ipGenerated) {
        generateSpoofedIP();
    }
    
    // Keep the fake values in GameData synchronized
    if (spoofName && usernameGenerated && Game.getFakeName() == nullptr && fakeNameHolder != nullptr) {
        Game.setFakeName(fakeNameHolder);
    }
    if (spoofDeviceId && deviceIdGenerated && Game.getFakeDeviceId() == nullptr && fakeDeviceIdHolder != nullptr) {
        Game.setFakeDeviceId(fakeDeviceIdHolder);
    }
    if (spoofXuid && xuidGenerated && Game.getFakeXuid() == nullptr && fakeXuidHolder != nullptr) {
        Game.setFakeXuid(fakeXuidHolder);
    }
    if (spoofIP && ipGenerated && Game.getFakeIP() == nullptr && fakeIPHolder != nullptr) {
        Game.setFakeIP(fakeIPHolder);
    }
    
    // Handle chat unblocking after timeout
    if (chatBlocked) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - chatBlockedTime);
        
        if (elapsed.count() >= CHAT_BLOCK_DURATION_SECONDS) {
            // Only unblock if we're not at last resort level
            if (currentDefenseLevel != DefenseLevel::LAST_RESORT_KICK) {
                unblockChat();
            }
        }
    }
    
    // Reset defense state if player is no longer in game
    if (!Game.isInGame()) {
        if (currentDefenseLevel != DefenseLevel::NORMAL) {
            resetDefenseState();
        }
    }
}

void Unbanner::onSendPacket(Packet* packet) {
    if (packet == nullptr) {
        return;
    }
    
    LocalPlayer* localPlayer = Game.getLocalPlayer();
    
    // PRIORITY 1: Aggressively block ban/kick commands
    if (packet->isInstanceOf<CommandRequestPacket>()) {
        CommandRequestPacket* cmdPacket = static_cast<CommandRequestPacket*>(packet);
        std::string cmd = cmdPacket->command.getText();
        
        if (detectBanKickCommand(cmd)) {
            // Activate defense if not already active
            if (currentDefenseLevel == DefenseLevel::NORMAL) {
                activateDefense();
            }
            
            // Block the command
            if (blockBanKickCommand(packet)) {
                // Successfully blocked
                return;
            } else {
                // Blocking failed, escalate
                escalateDefense();
            }
        }
    }
    
    // PRIORITY 2: Block chat messages when chat is blocked
    if (chatBlocked && packet->isInstanceOf<TextPacket>()) {
        TextPacket* textPacket = reinterpret_cast<TextPacket*>(packet);
        if (textPacket != nullptr) {
            // Block outgoing chat by clearing the message
            textPacket->message.setText("");
            
            logF("[Unbanner] Blocked outgoing chat message during protection period");
        }
        return;
    }
    
    // PRIORITY 3: Spoof username in chat messages if spoofing is enabled
    if (spoofName && usernameGenerated && !spoofedUsername.empty() && localPlayer != nullptr) {
        if (packet->isInstanceOf<TextPacket>()) {
            TextPacket* textPacket = reinterpret_cast<TextPacket*>(packet);
            if (textPacket != nullptr) {
                // Get the original player name to compare
                TextHolder* originalName = localPlayer->getNameTag();
                if (originalName != nullptr) {
                    std::string origNameStr = originalName->getText();
                    std::string sourceNameStr = textPacket->sourceName.getText();
                    
                    // If the source name contains the original player name, replace it with spoofed name
                    if (!origNameStr.empty() && sourceNameStr.find(origNameStr) != std::string::npos) {
                        textPacket->sourceName.setText(spoofedUsername);
                    }
                }
            }
        }
    }
}

void Unbanner::onPostRender(MinecraftUIRenderContext* ctx) {
    // Render chat blocked message if active
    if (chatBlocked) {
        renderChatBlockedMessage(ctx);
    }
    
    if (showButton) {
        renderUnbanButton(ctx);
    }
}

void Unbanner::renderChatBlockedMessage(MinecraftUIRenderContext* ctx) {
    // Null safety checks
    auto clientInstance = Game.getClientInstance();
    if (clientInstance == nullptr) return;
    auto guiData = clientInstance->getGuiData();
    if (guiData == nullptr) return;

    // Get window size for positioning
    Vec2 windowSize = guiData->windowSize;

    // Message box dimensions
    const float boxWidth = 450.0f;
    const float boxHeight = 50.0f;

    // Center horizontally, position near top
    float boxX = (windowSize.x - boxWidth) / 2.0f;
    float boxY = 80.0f;

    // Draw background box with semi-transparent orange
    Vec4 boxRect(boxX, boxY, boxX + boxWidth, boxY + boxHeight);
    MC_Color bgColor = MC_Color(255, 165, 0, 200);  // Orange with transparency
    DrawUtils::fillRectangle(boxRect, bgColor, 0.9f);

    // Draw border
    MC_Color borderColor = MC_Color(255, 100, 0);
    DrawUtils::drawRectangle(boxRect, borderColor, 1.0f, 2.0f);

    // Draw warning icon and text
    std::string warningText = "You have been temporarily blocked from chat, to avoid getting banned";
    float textWidth = DrawUtils::getTextWidth(&warningText, 0.9f);
    float textHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * 0.9f;
    
    Vec2 textPos(
        boxX + (boxWidth - textWidth) / 2.0f,
        boxY + (boxHeight - textHeight) / 2.0f
    );
    DrawUtils::drawText(textPos, &warningText, whiteColor, 0.9f, 1.0f);

    // Show remaining time
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - chatBlockedTime);
    int remaining = CHAT_BLOCK_DURATION_SECONDS - static_cast<int>(elapsed.count());
    if (remaining < 0) remaining = 0;
    
    std::string timerText = "Time remaining: " + std::to_string(remaining) + "s";
    float timerWidth = DrawUtils::getTextWidth(&timerText, 0.7f);
    Vec2 timerPos(
        boxX + (boxWidth - timerWidth) / 2.0f,
        boxY + boxHeight + 5.0f
    );
    DrawUtils::drawText(timerPos, &timerText, orangeColor, 0.7f, 1.0f);

    // Show defense level
    std::string defenseLevelText;
    switch (currentDefenseLevel) {
        case DefenseLevel::BLOCKING_COMMANDS:
            defenseLevelText = "Defense: Blocking Commands";
            break;
        case DefenseLevel::USERNAME_SPOOFING:
            defenseLevelText = "Defense: Username Spoofing";
            break;
        case DefenseLevel::LAST_RESORT_KICK:
            defenseLevelText = "Defense: DISCONNECT NOW!";
            break;
        default:
            defenseLevelText = "Defense: Active";
            break;
    }
    float defenseWidth = DrawUtils::getTextWidth(&defenseLevelText, 0.7f);
    Vec2 defensePos(
        boxX + (boxWidth - defenseWidth) / 2.0f,
        boxY + boxHeight + 20.0f
    );
    MC_Color defenseColor = (currentDefenseLevel == DefenseLevel::LAST_RESORT_KICK) ? redColor : xorionGreen;
    DrawUtils::drawText(defensePos, &defenseLevelText, defenseColor, 0.7f, 1.0f);
}

void Unbanner::renderUnbanButton(MinecraftUIRenderContext* ctx) {
    // Null safety checks
    auto clientInstance = Game.getClientInstance();
    if (clientInstance == nullptr) return;
    auto guiData = clientInstance->getGuiData();
    if (guiData == nullptr) return;

    // Get window size for positioning
    Vec2 windowSize = guiData->windowSize;

    // Button dimensions
    const float buttonWidth = 220.0f;
    const float buttonHeight = 35.0f;
    const float vpnInfoHeight = 20.0f;

    // Center button horizontally, position near bottom
    float buttonX = (windowSize.x - buttonWidth) / 2.0f;
    float buttonY = windowSize.y - buttonHeight - vpnInfoHeight - 60.0f;

    // Button rectangle
    Vec4 buttonRect(buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight);

    // Get mouse position with null check
    auto mousePtr = clientInstance->getMousePos();
    if (mousePtr == nullptr) return;
    Vec2 mousePos = *mousePtr;
    Vec2 windowSizeReal = guiData->windowSizeReal;
    mousePos = mousePos.div(windowSizeReal);
    mousePos = mousePos.mul(windowSize);

    // Check if mouse is hovering over button
    bool isHovered = buttonRect.contains(&mousePos);

    // Draw button background with Xorion green
    MC_Color buttonColor = isHovered ? xorionGreen.lerp(whiteColor, 0.2f) : xorionGreen;
    DrawUtils::fillRectangle(buttonRect, buttonColor, 1.0f);

    // Draw button text showing spoofed name (truncated if too long)
    std::string buttonText = "Regenerate Identity";
    if (usernameGenerated) {
        buttonText = spoofedUsername;
        if (buttonText.length() > 20) {
            buttonText = buttonText.substr(0, 17) + "...";
        }
    }
    float textWidth = DrawUtils::getTextWidth(&buttonText, 1.0f);
    float textHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
    Vec2 textPos(
        buttonX + (buttonWidth - textWidth) / 2.0f,
        buttonY + (buttonHeight - textHeight) / 2.0f
    );
    DrawUtils::drawText(textPos, &buttonText, whiteColor, 1.0f, 1.0f);

    // Draw VPN IP info below the button
    if (ipGenerated && spoofIP) {
        std::string vpnText = "VPN: " + spoofedIP;
        float vpnTextWidth = DrawUtils::getTextWidth(&vpnText, 0.8f);
        Vec2 vpnTextPos(
            buttonX + (buttonWidth - vpnTextWidth) / 2.0f,
            buttonY + buttonHeight + 4.0f
        );
        DrawUtils::drawText(vpnTextPos, &vpnText, xorionGreen, 0.8f, 1.0f);
    }

    // Handle click - regenerate all spoofed IDs
    if (isHovered && DrawUtils::shouldToggleLeftClick) {
        DrawUtils::shouldToggleLeftClick = false;
        generateAllSpoofedIds();
    }
}
