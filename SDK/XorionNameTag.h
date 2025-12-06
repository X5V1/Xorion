#pragma once
#include "../Xorion/DrawUtils.h"
#include "../Memory/GameData.h"

class XorionNameTag {
public:
    // Call this every frame during level render
    static void renderLocalPlayerName();
};
