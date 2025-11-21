#pragma once
#include "Vec3.h"
#include "Entity.h"
#include <string>

class LevelRenderer {
public:
    // Spawn a particle effect at a given position
    void spawnParticle(const char* particleName, const Vec3& pos);

    // Spawn a particle effect with velocity
    void spawnParticle(const char* particleName, const Vec3& pos, const Vec3& velocity);

    // Render block breaking particles (e.g. cracks)
    void renderBlockBreaking(const Vec3& blockPos, int blockId);

    // Render weather effects (rain, snow) at a position
    void renderWeather(const Vec3& pos, bool isRaining, bool isSnowing);

    // Render entity hit particles (like critical hits, mace impact, etc.)
    void renderEntityHit(Entity* entity, const char* particleName);

    // Utility: clear all particles
    void clearParticles();

    // Utility: reload renderer (e.g. after resource pack change)
    void reloadRenderer();

    // Debug: log particle spawn
    void debugLogParticle(const std::string& particleName, const Vec3& pos);
};
