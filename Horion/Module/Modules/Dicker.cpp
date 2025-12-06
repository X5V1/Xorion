#include "Dicker.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/LocalPlayer.h"
#include "../../../SDK/GameMode.h"
#include "../../../Utils/HMath.h"

Dicker::Dicker()
    : IModule(0x0, Category::WORLD, "Automatically builds Dicks") {
    registerIntSetting("Delay", &delay, delay, 1, 40);
    registerIntSetting("Spacing", &spacing, spacing, 1, 10);
}

Dicker::~Dicker() {}

const char* Dicker::getModuleName() {
    return "Dicker";
}

void Dicker::placePatternAt(GameMode* gm, const vec3_ti& basePos) {
    // Crafting table layout:
    // Row1: [ ] [X] [ ]
    // Row2: [ ] [X] [ ]
    // Row3: [X] [ ] [X]

    std::vector<vec3_ti> offsets = {
        vec3_ti(0, 2, 0),   // row 1 middle
        vec3_ti(0, 1, 0),   // row 2 middle
        vec3_ti(-1, 0, 0),  // row 3 left
        vec3_ti(1, 0, 0)    // row 3 right
    };

    for (auto& off : offsets) {
        vec3_ti target_temp = vec3_ti(basePos.x + off.x, basePos.y + off.y, basePos.z + off.z);
        Vec3i target = Vec3i(target_temp.x, target_temp.y, target_temp.z);
        gm->buildBlock(&target, 1, false); // 1 = face (up)
    }
}

void Dicker::onTick(GameMode* gm) {
    if (!g_Data.isInGame()) return;
    if (tickCounter++ < delay) return;
    tickCounter = 0;

    LocalPlayer* lp = g_Data.getLocalPlayer();
    if (!lp) return;

    Vec3 posFloat = lp->getPos();
    Vec3 floored = posFloat.floor();
    vec3_ti playerBlockPos = vec3_ti(static_cast<int>(floored.x), static_cast<int>(floored.y), static_cast<int>(floored.z));

    // Facing direction
    float yaw = lp->getActorHeadRotationComponent()->rot.y * (PI / 180.f);
    vec3_t forward = vec3_t(-sinf(yaw), 0.0f, cosf(yaw));
    vec3_t right = vec3_t(forward.z, 0.0f, -forward.x); // perpendicular right vector

    // Base position: one block in front
    vec3_ti basePos_temp = vec3_ti(playerBlockPos.x + (int)forward.x, playerBlockPos.y, playerBlockPos.z + (int)forward.z);
    vec3_ti basePos = basePos_temp;

    // Keep placing patterns infinitely to the right
    for (int i = 0; i < 50; i++) { // 50 is arbitrary "infinite" cap per tick
        vec3_ti shifted = vec3_ti(basePos.x + (int)(right.x * spacing * i), basePos.y, basePos.z + (int)(right.z * spacing * i));
        placePatternAt(gm, shifted);
    }
}
