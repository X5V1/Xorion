#include "AutoCrystal.h"
#include "GameData.h"
#include "Utils.h"

AutoCrystal::AutoCrystal() : IModule(0, Category::WORLD, "Places crystal, then explodes crystal") {}
AutoCrystal::~AutoCrystal() {}

const char* AutoCrystal::getModuleName() {
    return "AutoCrystal";
}

void AutoCrystal::onTick(GameMode* gm) {
    auto player = g_Data.getLocalPlayer();
    if (!player) return;

    // Only run every 20 ticks (1 second)
    if (++tickCounter < 20) return;
    tickCounter = 0;

    vec3_ti basePos(
        (int)floor(player->getPos()->x),
        (int)floor(player->getPos()->y),
        (int)floor(player->getPos()->z)
    );

    if (!crystalPlaced) {
        // Step 1: place obsidian at feet
        Utils::placeBlock(basePos, minecraft:obsidian);

        // Step 2: place crystal on top
        crystalPos = basePos.add(0, 1, 0);
        Utils::placeBlock(crystalPos, minecraft:end_crystal);

        crystalPlaced = true;
    } else {
        // Step 3: break the crystal after delay
        Utils::destroyBlock(crystalPos);
        crystalPlaced = false;
    }
}
