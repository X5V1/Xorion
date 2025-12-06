#pragma once
#include "SDK/Vec2.h"
#include "SDK/Vec4.h"
#include "SDK/Font.h"
#include <memory>
#include <map>

// vec4_t is MC_Color rectangle representation
using vec4_t = Vec4;

struct ComponentInfo {
    int id;
    ComponentInfo(int id);
    virtual ~ComponentInfo() = default;  // Make polymorphic for dynamic_cast
};

struct ButtonInfo : ComponentInfo {
    vec2_t pos;
    vec2_t size;
    bool centered = false;
    bool canClickB = false;
    float padding = 5.0f;
    
    ButtonInfo(int id, vec2_t pos, bool centered);
    void calculateSize(const char* txt);
    bool isInSelectableSurface(vec2_t mouse);
    vec4_t getSelectableSurface();
    void draw(vec2_t mousePos, const char* label);
    void updatePos(vec2_t newPos) { pos = newPos; }
    bool canClick() const { return canClickB; }
};

struct MouseButton {
    bool isDown = false;
    bool nextIsDown = false;
    bool isClicked = false;
    
    void update() {
        isClicked = !isDown && nextIsDown;
        isDown = nextIsDown;
        nextIsDown = false;
    }
    
    bool trySteal() {
        if (isClicked) {
            isClicked = false;
            return true;
        }
        return false;
    }
};

class ImmediateGui {
public:
    vec2_t mousePos;
    MouseButton leftMb;
    MouseButton rightMb;
    std::map<int, std::shared_ptr<ComponentInfo>> components;

    void onMouseClickUpdate(int key, bool isDown);
    void startFrame();
    void endFrame();
    bool Button(const char* label, vec2_t pos, bool centered = false);
    
    bool isInBox(const vec2_t &pos, const vec2_t &size, const vec2_t &mousePos);
    void renderButton(ButtonInfo &btn);
};
