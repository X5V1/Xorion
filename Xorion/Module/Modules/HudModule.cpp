#include "HudModule.h"
#include "../../DrawUtils.h"
#include "../ModuleManager.h"
#include "../../../Utils/ClientColors.h"
#include "../../../SDK/Font.h"

HudModule::HudModule() : IModule(0, Category::CLIENT, "Displays things like the ArrayList/TabGUI.") {
    registerBoolSetting("TabGui", &tabgui, tabgui);
    registerBoolSetting("ClickToggle", &clickToggle, clickToggle);
    // Removed Watermark toggle — now handled by Watermark.cpp
    registerBoolSetting("Coordinates", &coordinates, coordinates);
    registerBoolSetting("Show Keybinds", &keybinds, keybinds);
    registerBoolSetting("Show ArmorHUD", &displayArmor, displayArmor);
    registerBoolSetting("Keystrokes", &keystrokes, keystrokes);
    registerBoolSetting("Show FPS", &fps, fps);
    registerBoolSetting("Show CPS", &cps, cps);
    registerBoolSetting("Always show", &alwaysShow, alwaysShow);
    registerFloatSetting("Scale", &scale, scale, 0.5f, 1.5f);
}

HudModule::~HudModule() {}

const char* HudModule::getModuleName() {
    return "HUD";
}

void HudModule::drawKeystroke(char key, const Vec2& pos) {
    std::string keyString = Utils::getKeybindName(key);
    GameSettingsInput* input = Game.getClientInstance()->getGameSettingsInput();

    if (key == *input->spaceBarKey) {
        keyString = "-";
        Vec4 rectPos(pos.x, pos.y, pos.x + 64.f, pos.y + 15.f);
        Vec2 textPos((rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString) / 2.f),
                     rectPos.y + 7.f - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.f);

        DrawUtils::fillRectangle(rectPos, GameData::isKeyDown(key) ? ClientColors::keyStrokeDownColor : ClientColors::keyStrokeUpColor, 1.f);
        DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f);
    } else {
        Vec4 rectPos(pos.x, pos.y, pos.x + 20.f, pos.y + 20.f);
        Vec2 textPos((rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString) / 2.f),
                     rectPos.y + 10.f - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.f);

        DrawUtils::fillRectangle(rectPos, GameData::isKeyDown(key) ? ClientColors::keyStrokeDownColor : ClientColors::keyStrokeUpColor, 1.f);
        DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f);
    }
}

void HudModule::drawMouseKeystroke(Vec2 pos, std::string keyString) {
    Vec4 rectPos(pos.x, pos.y + 2, pos.x + 31.f, pos.y + 22.f);
    if (keyString == "LMB")
        DrawUtils::fillRectangle(rectPos, g_Data.isLeftClickDown() ? ClientColors::keyStrokeDownColor : ClientColors::keyStrokeUpColor, 1.f);
    else if (keyString == "RMB")
        DrawUtils::fillRectangle(rectPos, g_Data.isRightClickDown() ? ClientColors::keyStrokeDownColor : ClientColors::keyStrokeUpColor, 1.f);

    Vec2 textPos((rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString) / 2.f),
                 rectPos.y + 10.f - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.f);
    DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f);
}

void HudModule::onPostRender(MinecraftUIRenderContext* renderCtx) {
    Vec2 windowSize = Game.getClientInstance()->getGuiData()->windowSize;
    float f = 10.f * scale;
    std::string tempStr("Movement");
    float len = DrawUtils::getTextWidth(&tempStr, scale) + 7.f;
    float startY = tabgui ? 7 * f : 0.f;

    { // FPS
        if (!(g_Data.getLocalPlayer() == nullptr || !fps)) {
            std::string fpsText = "FPS: " + std::to_string(Game.getFPS());
            Vec4 rectPos = Vec4(2.5f, startY + 5.f * scale, len, startY + 15.f * scale);
            Vec2 textPos = Vec2(rectPos.x + 1.5f, rectPos.y + 1.f);
            DrawUtils::fillRectangle(rectPos, ClientColors::fpsBackgroundColor, 1.f);
            DrawUtils::drawText(textPos, &fpsText, MC_Color(200, 200, 200), scale);
            startY += f;
        }
    }

    { // CPS
        if (!(g_Data.getLocalPlayer() == nullptr || !cps)) {
            std::string cpsText = "CPS: " + std::to_string(Game.getLeftCPS()) + " - " + std::to_string(Game.getRightCPS());
            Vec4 rectPos = Vec4(2.5f, startY + 5.f * scale, len, startY + 15.f * scale);
            Vec2 textPos = Vec2(rectPos.x + 1.5f, rectPos.y + 1.f);
            DrawUtils::fillRectangle(rectPos, ClientColors::cpsBackgroundColor, 1.f);
            DrawUtils::drawText(textPos, &cpsText, MC_Color(200, 200, 200), scale);
            startY += f;
        }
    }

    { // Coordinates
        if (!(g_Data.getLocalPlayer() == nullptr || !coordinates)) {
            Vec3 pos = g_Data.getLocalPlayer()->getPos();
            std::string coords = "X: " + std::to_string((int)floorf(pos.x)) +
                                 "\nY: " + std::to_string((int)floorf(pos.y)) +
                                 "\nZ: " + std::to_string((int)floorf(pos.z));
            Vec4 rectPos = Vec4(2.5f, startY + 5.f * scale, len, startY + 35.f * scale);
            Vec2 textPos = Vec2(rectPos.x + 1.5f, rectPos.y + 1.f);
            DrawUtils::fillRectangle(rectPos, ClientColors::coordinatesBackgroundColor, 1.f);
            DrawUtils::drawText(textPos, &coords, MC_Color(200, 200, 200), scale);
        }
    }

    { // ArmorHUD
        if (!(g_Data.getLocalPlayer() == nullptr || !displayArmor || !g_Data.canUseMoveKeys())) {
            static float constexpr scale = 1.f;
            static float constexpr opacity = 0.75f;
            static float constexpr spacing = scale + 15.f;
            LocalPlayer* player = g_Data.getLocalPlayer();
            float x = windowSize.x / 2.f + 5.f;
            float y = windowSize.y - 57.5f;
            for (int i = 0; i < 4; i++) {
                ItemStack* stack = player->getArmor(i);
                if (stack->isValid) {
                    DrawUtils::drawItem(stack, Vec2(x, y), opacity, scale, stack->isEnchanted());
                    x += scale * spacing;
                }
            }
            PlayerSupplies* supplies = g_Data.getLocalPlayer()->getSupplies();
            ItemStack* item = supplies->inventory->getByGlobalIndex(supplies->selectedHotbarSlot);
            if (item->isValid)
                DrawUtils::drawItem(item, Vec2(x, y), opacity, scale, item->isEnchanted());
        }
    }

       { // Keystrokes
        if (!(g_Data.getLocalPlayer() == nullptr || !keystrokes || !g_Data.canUseMoveKeys())) {
            GameSettingsInput* input = Game.getClientInstance()->getGameSettingsInput();
            HudModule::drawKeystroke(*input->forwardKey, Vec2(32.f, windowSize.y - 84));
            HudModule::drawKeystroke(*input->leftKey, Vec2(10.f, windowSize.y - 62));
            HudModule::drawKeystroke(*input->backKey, Vec2(32.f, windowSize.y - 62));
            HudModule::drawKeystroke(*input->rightKey, Vec2(54.f, windowSize.y - 62));
            HudModule::drawKeystroke(*input->spaceBarKey, Vec2(10.f, windowSize.y - 40));
            HudModule::drawMouseKeystroke(Vec2(10.f, windowSize.y - 25), "LMB");
            HudModule::drawMouseKeystroke(Vec2(43.f, windowSize.y - 25), "RMB");
        }
    }
} // <- closes onPostRender

