#pragma once

#include "../Utils/Utils.h"

// Forward declare to avoid heavy includes
struct PlayerAuthInputPacket;

// Use struct consistently across SDK to avoid C4099 warnings.
struct MoveInputHandler {
    // Previous-frame states
    BUILD_ACCESS(this, bool, isSneakDownPrev, 0x0);
    BUILD_ACCESS(this, bool, isJumpingPrev,   0x6);
    BUILD_ACCESS(this, bool, isSprintingPrev, 0x7);
    BUILD_ACCESS(this, bool, forwardPrev,     0xA);
    BUILD_ACCESS(this, bool, backwardPrev,    0xB);
    BUILD_ACCESS(this, bool, leftPrev,        0xC);
    BUILD_ACCESS(this, bool, rightPrev,       0xD);

    // Current-frame states
    BUILD_ACCESS(this, bool, isSneakDown,     0x20);
    BUILD_ACCESS(this, bool, isJumping,       0x26);
    BUILD_ACCESS(this, bool, isSprinting,     0x27);
    BUILD_ACCESS(this, bool, forward,         0x2A);
    BUILD_ACCESS(this, bool, backward,        0x2B);
    BUILD_ACCESS(this, bool, left,            0x2C);
    BUILD_ACCESS(this, bool, right,           0x2D);

    // Analog movement values
    BUILD_ACCESS(this, float, sideMovement,    0x48);
    BUILD_ACCESS(this, float, forwardMovement, 0x4C);

    inline bool isPressed() const {
        return forward || backward || left || right;
    }
};
