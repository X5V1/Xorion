#pragma once
#include "SDK/Vec2.h"
#include "SDK/Vec3.h"

struct JVector2 {
    vec2_t vec;
    void set(float x, float y) { vec.x = x; vec.y = y; }
};

struct JVector3 {
    vec3_t vec;
    void set(float x, float y, float z) { vec.x = x; vec.y = y; vec.z = z; }
};

namespace Script {
    enum Error {
        Invalid,
        Other
    };
}
