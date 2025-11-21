// Player.h
#pragma once
#include <string>
#include "Vec3.h"
#include "ItemStack.h"

// Base player wrapper; calls into game via resolved signatures.
class Entity; // forward declaration to avoid heavy includes

class Player : public Entity {
public:
    explicit Player(void* mcPlayerPtr = nullptr);
    virtual ~Player();

    std::string getName() const;
    ItemStack   getHeldItem() const;
    Vec3        getPos() const;
    void        setPos(const Vec3& pos);
    float       getHealth() const;
    void        setHealth(float health);

    inline void* getMcPlayer() const { return mcPlayerPtr; }

protected:
    void* mcPlayerPtr;

    static const char* (*Player_getName_fn)(void* mcPlayer);
    static void*       (*Player_getItemStackInHand_fn)(void* mcPlayer);
    static Vec3        (*Player_getPos_fn)(void* mcPlayer);
    static void        (*Player_setPos_fn)(void* mcPlayer, const Vec3& pos);
    static float       (*Player_getHealth_fn)(void* mcPlayer);
    static void        (*Player_setHealth_fn)(void* mcPlayer, float health);

    void ensureIntegration() const;
};
