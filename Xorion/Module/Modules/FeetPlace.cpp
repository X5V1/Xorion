#include "FeetPlace.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/GameMode.h"
#include "../../../SDK/LocalPlayer.h"
#include "../../../Utils/HMath.h"
#include <cmath>

void FeetPlace::onTick(GameMode* gm) {
    if (!gm || !g_Data.getLocalPlayer()) return;

    LocalPlayer* lp = g_Data.getLocalPlayer();
    Vec3 pos = lp->getPos();

    // Get yaw from ActorHeadRotationComponent
    auto headRot = lp->getActorHeadRotationComponent();
    if (!headRot) return;
    
    float yawRad = headRot->rot.y * 3.14159265f / 180.0f;
    
    // Compute block directly in front of feet
    int targetX = static_cast<int>(std::floor(pos.x - std::sin(yawRad)));
    int targetY = static_cast<int>(std::floor(pos.y - 1)); // feet level
    int targetZ = static_cast<int>(std::floor(pos.z + std::cos(yawRad)));

    Vec3i target(targetX, targetY, targetZ);

    // Use buildBlock to place block (face 1 = top face)
    gm->buildBlock(&target, 1, false);
}
