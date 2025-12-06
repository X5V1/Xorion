#include "HudEditor.h"
#include "../../../Memory/GameData.h"

HudEditor::HudEditor() : IModule(0x0, Category::CLIENT, "Drag and drop HUD elements") {
    registerBoolSetting("Snap to grid", &snapToGrid, snapToGrid);
    registerFloatSetting("Grid size", &gridSize, gridSize, 2.0f, 32.0f);
    registerBoolSetting("Show outlines", &showOutlines, showOutlines);

    // Example elements (replace or expand with your actual HUD modules)
    registerElement("Watermark", vec2_t(10, 10), vec2_t(80, 20));
    registerElement("TargetHUD", vec2_t(200, 200), vec2_t(160, 60));
}

HudEditor::~HudEditor() {}

const char* HudEditor::getModuleName() {
    return "HudEditor";
}

void HudEditor::registerElement(const std::string& name, const vec2_t& pos, const vec2_t& size) {
    elements.push_back({name, pos, size, true, true});
}

void HudEditor::onPostRender(MinecraftUIRenderContext* renderCtx) {
    if (!g_Data.isInGame()) return;

    for (auto& e : elements) {
        if (!e.enabled) continue;

        if (showOutlines) {
            MC_Color col = MC_Color(255, 255, 255, 80);
            Vec4 rect(e.pos.x, e.pos.y, e.pos.x + e.size.x, e.pos.y + e.size.y);
            DrawUtils::drawRectangle(rect, col, 1.0f, 0.5f);
            std::string name = e.name;
            DrawUtils::drawText(e.pos.add(vec2_t(2, 2)), &name, MC_Color(200, 200, 200), 0.8f);
        }
    }
}

// onMouseClickUpdate removed - not in base class
/*
void HudEditor::onMouseClickUpdate(int mouseX, int mouseY, int button, bool isDown) {
    vec2_t mouse((float)mouseX, (float)mouseY);

    if (isDown && button == 1) { // left click down
        for (auto& e : elements) {
            vec2_t min = e.pos;
            vec2_t max = e.pos.add(e.size);
            if (mouse.x >= min.x && mouse.x <= max.x && mouse.y >= min.y && mouse.y <= max.y) {
                dragging = &e;
                dragOffset = mouse.sub(e.pos);
                break;
            }
        }
    } else if (!isDown && button == 1) { // left click up
        dragging = nullptr;
    }

    if (dragging && isDown && button == 0) { // mouse move while dragging
        dragging->pos = mouse.sub(dragOffset);
        if (snapToGrid) {
            dragging->pos.x = roundf(dragging->pos.x / gridSize) * gridSize;
            dragging->pos.y = roundf(dragging->pos.y / gridSize) * gridSize;
        }
    }
}
*/
