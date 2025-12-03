#pragma once
#include "DrawUtils.h"
#include "GameData.h"

class XorionNameTag {
public:
    // Call this every frame during level render
    static void renderLocalPlayerName();
};
