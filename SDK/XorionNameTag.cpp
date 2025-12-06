#include "XorionNameTag.h"
#include "Font.h"   // or whatever your client uses to manage fonts

void XorionNameTag::renderLocalPlayerName() {
    LocalPlayer* lp = g_Data.getLocalPlayer();
    if (lp == nullptr) return;

    // Position above the player's head
    Vec3 pos = lp->getPos();
    pos.y += lp->getAABBShapeComponent()->size.y + 0.5f;

    // Grab the player's username
    std::string name = lp->getNameTag()->getText();

    // TODO: getFontRepository not available
    // Draw the text at screen position
    Vec2 screenPos(pos.x, pos.y);  // TODO: Convert world to screen coordinates
    DrawUtils::drawText(screenPos, &name, MC_Color(255, 255, 255), 1.0f);
}
