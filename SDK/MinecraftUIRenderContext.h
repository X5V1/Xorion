#pragma once

#include <cstdint>
#include "Vec3.h"
#include "TextHolder.h"

// Minimal self-contained render context.
// Remove any Utils::CallVFunc references to stop compilation errors.
// Modules will only use this type in hook signatures; drawing can be routed via DrawUtils.

struct MinecraftUIRenderContext {
    void* nativeCtx = nullptr;

    // No dependencies on Utils::CallVFunc — provide stubs or forwarders you can wire later.
    inline void setColor(float r, float g, float b, float a) { (void)r; (void)g; (void)b; (void)a; }
    inline void fillRect(float x1, float y1, float x2, float y2) { (void)x1; (void)y1; (void)x2; (void)y2; }
    inline void drawText(float x, float y, const TextHolder& text, float scale = 1.0f) { (void)x; (void)y; (void)text; (void)scale; }
    inline void drawLine(float x1, float y1, float x2, float y2, float thickness = 1.0f) { (void)x1; (void)y1; (void)x2; (void)y2; (void)thickness; }
    inline void drawBox(const Vec3& start, const Vec3& end, float thickness = 0.4f) { (void)start; (void)end; (void)thickness; }
};
