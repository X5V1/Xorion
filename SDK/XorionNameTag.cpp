#include "XorionNameTag.h"
#include "FontRepository.h"   // or whatever your client uses to manage fonts

void XorionNameTag::renderLocalPlayerName() {
    C_LocalPlayer* lp = g_Data.getLocalPlayer();
    if (lp == nullptr) return;

    // Position above the player's head
    vec3_t pos = *lp->getPos();
    pos.y += lp->height + 0.5f;

    // Grab the player's username
    std::string name = lp->getNameTag()->getText();

    // Use the Xorion font (replace with your actual font object)
    C_Font* xorionFont = g_Data.getFontRepository()->getFont("Xorion"); 
    if (xorionFont == nullptr) {
        // fallback to default if Xorion font not loaded
        xorionFont = g_Data.getFontRepository()->getFont("Default");
    }

    // Draw the text
    DrawUtils::drawText(pos, &name, MC_Color(255, 255, 255), 1.0f, xorionFont);
}
