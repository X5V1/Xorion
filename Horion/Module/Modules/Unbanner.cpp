#include "Unbanner.h"
#include "../../DrawUtils.h"
#include "../../../Memory/GameData.h"

// Xorion green color (same as xorion-banner.png)
static const MC_Color xorionGreen = MC_Color(37, 164, 64);
static const MC_Color whiteColor = MC_Color(255, 255, 255);

Unbanner::Unbanner()
    : IModule(0, Category::MISC, "Attempt to unban from servers. Displays a green button.") {
    registerBoolSetting("AutoReconnect", &autoReconnect, autoReconnect);
    registerIntSetting("RetryDelay", &retryDelay, retryDelay, 5, 200);
    registerBoolSetting("ShowButton", &showButton, showButton);
}

Unbanner::~Unbanner() {}

const char* Unbanner::getModuleName() {
    return "Unbanner";
}

void Unbanner::onEnable() {
    tickCounter = 0;
}

void Unbanner::onDisable() {
    tickCounter = 0;
}

void Unbanner::onTick(GameMode* gm) {
    if (!autoReconnect) return;

    tickCounter++;
    if (tickCounter >= retryDelay) {
        tickCounter = 0;
        tryUnban();
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
    const float buttonWidth = 150.0f;
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

    // Draw button text "Try Unban Me" in white, centered
    std::string buttonText = "Try Unban Me";
    float textWidth = DrawUtils::getTextWidth(&buttonText, 1.0f);
    float textHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
    Vec2 textPos(
        buttonX + (buttonWidth - textWidth) / 2.0f,
        buttonY + (buttonHeight - textHeight) / 2.0f
    );
    DrawUtils::drawText(textPos, &buttonText, whiteColor, 1.0f, 1.0f);

    // Handle click
    if (isHovered && DrawUtils::shouldToggleLeftClick) {
        DrawUtils::shouldToggleLeftClick = false;
        tryUnban();
    }
}

void Unbanner::tryUnban() {
    // Attempt to reconnect/unban logic
    // This is a placeholder - actual implementation would depend on server API
    // For now, we try to trigger a reconnect if disconnected

    auto clientInstance = Game.getClientInstance();
    if (clientInstance != nullptr) {
        // TODO: Implement actual unban/reconnect logic
        // This might involve:
        // 1. Checking if currently disconnected
        // 2. Attempting to reconnect to the last server
        // 3. Clearing ban-related client state if applicable
    }
}
