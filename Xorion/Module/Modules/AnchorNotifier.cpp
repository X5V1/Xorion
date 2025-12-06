#include "AnchorNotifier.h"
#include "GameData.h"
#include "Utils.h"

AnchorNotifier::AnchorNotifier() : IModule(0, Category::WORLD, "Warns if charged anchors nearby") {}
AnchorNotifier::~AnchorNotifier() {}

const char* AnchorNotifier::getModuleName() {
    return "AnchorNotifier";
}

void AnchorNotifier::onTick(GameMode* gm) {
    auto player = g_Data.getLocalPlayer();
    if (!player) return;

    vec3_ti playerPos((int)floor(player->getPos()->x),
                      (int)floor(player->getPos()->y),
                      (int)floor(player->getPos()->z));

    int radius = 6; // scan radius
    for (int x = -radius; x <= radius; x++) {
        for (int y = -radius; y <= radius; y++) {
            for (int z = -radius; z <= radius; z++) {
                vec3_ti checkPos = playerPos.add(x,y,z);
                auto block = g_Data.getRegion()->getBlock(checkPos);

                if (block->toLegacy()->blockId == RESPAWN_ANCHOR_BLOCK_ID) {
                    int charges = block->getData(); // depends on your API
                    if (charges == 4) {
                        g_Data.getGuiData()->displayClientMessageF(
                            "⚠ Fully charged anchor detected at %d %d %d",
                            checkPos.x, checkPos.y, checkPos.z
                        );
                    }
                }
            }
        }
    }
}
