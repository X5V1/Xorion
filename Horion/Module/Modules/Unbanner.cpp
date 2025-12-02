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
    : IModule(0, Category::MISC, "Spoofs your identity to help avoid bans.") {
    registerBoolSetting("SpoofName", &spoofName, spoofName);
    registerBoolSetting("SpoofDeviceId", &spoofDeviceId, spoofDeviceId);
    registerBoolSetting("SpoofXuid", &spoofXuid, spoofXuid);
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
    
    // XUID is typically a 16-digit number starting with "2" for Xbox Live accounts
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

void Unbanner::generateAllSpoofedIds() {
    if (spoofName) generateSpoofedUsername();
    if (spoofDeviceId) generateSpoofedDeviceId();
    if (spoofXuid) generateSpoofedXuid();
    
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
    
    usernameGenerated = false;
    deviceIdGenerated = false;
    xuidGenerated = false;
    spoofedUsername.clear();
    spoofedDeviceId.clear();
    spoofedXuid.clear();
    
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

    // Center button horizontally, position near bottom
    float buttonX = (windowSize.x - buttonWidth) / 2.0f;
    float buttonY = windowSize.y - buttonHeight - 60.0f;

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

    // Handle click - regenerate all spoofed IDs
    if (isHovered && DrawUtils::shouldToggleLeftClick) {
        DrawUtils::shouldToggleLeftClick = false;
        generateAllSpoofedIds();
    }
}
