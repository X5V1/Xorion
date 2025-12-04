#include "Unbanner.h"
#include "../../DrawUtils.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/Packet.h"
#include "../../../SDK/TextHolder.h"
#include "../../../Utils/Utils.h"
#include <random>
#include <ctime>
#include <sstream>
#include <iomanip>

// Xorion green color (same as xorion-banner.png)
static const MC_Color xorionGreen = MC_Color(37, 164, 64);
static const MC_Color whiteColor = MC_Color(255, 255, 255);

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
    : IModule(0, Category::MISC, "Spoofs your identity to help avoid bans. Includes VPN-like IP spoofing.") {
    registerBoolSetting("SpoofName", &spoofName, spoofName);
    registerBoolSetting("SpoofDeviceId", &spoofDeviceId, spoofDeviceId);
    registerBoolSetting("SpoofXuid", &spoofXuid, spoofXuid);
    registerBoolSetting("SpoofIP", &spoofIP, spoofIP);
    registerBoolSetting("ShowButton", &showButton, showButton);
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

void Unbanner::onEnable() {
    generateAllSpoofedIds();
    
    auto clientInstance = Game.getClientInstance();
    if (clientInstance != nullptr) {
        auto guiData = clientInstance->getGuiData();
        if (guiData != nullptr) {
            guiData->displayClientMessageF("[Unbanner] Enabled - Spoofed identities generated.");
            guiData->displayClientMessageF("[Unbanner] Reconnect to apply spoofed IDs to the server.");
        }
    }
}

void Unbanner::onDisable() {
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
            guiData->displayClientMessageF("[Unbanner] Disabled - Identity spoofing deactivated.");
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
}

void Unbanner::onSendPacket(Packet* packet) {
    if (!spoofName || !usernameGenerated || spoofedUsername.empty()) {
        return;
    }
    
    LocalPlayer* localPlayer = Game.getLocalPlayer();
    if (localPlayer == nullptr) {
        return;
    }
    
    // Intercept TextPacket (chat messages) and modify the source name
    if (packet->isInstanceOf<TextPacket>()) {
        TextPacket* textPacket = reinterpret_cast<TextPacket*>(packet);
        if (textPacket == nullptr) {
            return;
        }
        
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

void Unbanner::onPostRender(MinecraftUIRenderContext* ctx) {
    if (showButton) {
        renderUnbanButton(ctx);
    }
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
    float textHeight = 10.0f; // TODO: Font type removed in 1.21.123 - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
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
