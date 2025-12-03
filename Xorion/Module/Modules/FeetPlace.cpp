#include "FeetPlace.h"
#include <cmath>

void FeetPlace::onTick(GameMode* gm) {
    if (!gm || !g_Data.getLocalPlayer()) return;

    LocalPlayer* lp = g_Data.getLocalPlayer();
    Vec3 pos = lp->getPos();

    // Compute block directly in front of feet
    float yawRad = lp->getYaw() * 3.14159265f / 180.0f;
    int targetX = static_cast<int>(std::floor(pos.x - std::sin(yawRad)));
    int targetY = static_cast<int>(std::floor(pos.y - 1)); // feet level
    int targetZ = static_cast<int>(std::floor(pos.z + std::cos(yawRad)));

    BlockPos target(targetX, targetY, targetZ);

    // Only place if air
    if (gm->getWorld()->isAirBlock(target)) {
        gm->placeBlock(target, ItemStack(ItemDescriptor{"minecraft:obsidian"}));
    }
}
