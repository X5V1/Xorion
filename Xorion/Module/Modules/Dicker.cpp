#include "Dicker.h"
#include "../../SDK/GameData.h"
#include "../../SDK/C_LocalPlayer.h"
#include "../../SDK/C_GameMode.h"

Dicker::Dicker()
    : IModule(0x0, Category::WORLD, "Automatically builds Dicks") {
    registerIntSetting("Delay", &delay, delay, 1, 40);
    registerIntSetting("Spacing", &spacing, spacing, 1, 10);
}

Dicker::~Dicker() {}

const char* Dicker::getModuleName() {
    return "Dicker";
}

void Dicker::placePatternAt(C_GameMode* gm, const vec3_ti& basePos) {
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
        vec3_ti target = basePos.add(off);
        gm->buildBlock(&target, 1); // 1 = face (up)
    }
}

void Dicker::onTick(C_GameMode* gm) {
    if (!g_Data.isInGame()) return;
    if (tickCounter++ < delay) return;
    tickCounter = 0;

    C_LocalPlayer* lp = g_Data.getLocalPlayer();
    if (!lp) return;

    vec3_ti playerBlockPos = vec3_ti(lp->getPos()->floor());

    // Facing direction
    float yaw = lp->yaw * (PI / 180.f);
    vec3_t forward(-sinf(yaw), 0, cosf(yaw));
    vec3_t right(forward.z, 0, -forward.x); // perpendicular right vector

    // Base position: one block in front
    vec3_ti basePos = playerBlockPos.add(vec3_ti((int)forward.x, 0, (int)forward.z));

    // Keep placing patterns infinitely to the right
    for (int i = 0; i < 50; i++) { // 50 is arbitrary "infinite" cap per tick
        vec3_ti shifted = basePos.add(vec3_ti((int)(right.x * spacing * i), 0, (int)(right.z * spacing * i)));
        placePatternAt(gm, shifted);
    }
}
