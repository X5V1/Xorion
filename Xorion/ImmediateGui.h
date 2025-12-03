#pragma once
#include "SDK/Vec2.h"

struct ButtonInfo {
    vec2_t pos;
    vec2_t size;
    void draw(); // implemented in .cpp
    vec2_t getSelectableSurface();
};

class ImmediateGui {
public:
    vec2_t mousePos;

    bool isInBox(const vec2_t &pos, const vec2_t &size, const vec2_t &mousePos);
    void renderButton(ButtonInfo &btn);
};
