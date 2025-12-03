#include "MaceHit.h"
#include "../../../SDK/Entity.h"
#include "../../../Utils/Logger.h"
#include "../../../SDK/GameMode.h"
#include "../../../SDK/Player.h"
#include "../../../SDK/LevelRenderer.h"   // for particles
#include "../../../SDK/ClientInstance.h"  // for sound events

MaceHit::MaceHit() 
    : IModule(0, Category::COMBAT, "Every melee hit acts like a mace drop from 10 blocks") {
}

MaceHit::~MaceHit() {}

const char* MaceHit::getModuleName() {
    return "MaceHit";
}

void MaceHit::onAttack(C_Entity* attackedEntity) {
    if (attackedEntity == nullptr) return;

    // Apply mace-style damage: +5 (10 blocks * 0.5)
    float extraDamage = 5.0f;

    GameMode* gm = g_Data.getGameMode();
    if (gm != nullptr) {
        gm->attack(attackedEntity);          // normal attack
        attackedEntity->damage(extraDamage); // extra mace damage
    }

    // Apply knockback
    C_Player* player = g_Data.getLocalPlayer();
    if (player != nullptr) {
        Vec3 attackerPos = player->getPos();
        Vec3 targetPos = attackedEntity->getPos();
        Vec3 dir = targetPos.sub(attackerPos).normalize();

        float kbStrength = 1.2f;
        attackedEntity->velocity = attackedEntity->velocity.add(dir.mul(kbStrength));
    }

    // Spawn mace particles at target position
    ClientInstance* ci = g_Data.getClientInstance();
    if (ci != nullptr) {
        LevelRenderer* lr = ci->getLevelRenderer();
        if (lr != nullptr) {
            Vec3 pos = attackedEntity->getPos();
            lr->spawnParticle("minecraft:mace_hit", pos);
        }
    }

    // Play mace hit sound
    if (ci != nullptr) {
        Vec3 pos = attackedEntity->getPos();
        ci->playSound("item.mace.hit", pos, 1.0f, 1.0f);
    }

    logF("[MaceHit] Applied mace damage + knockback + particles + sound to %s", 
         attackedEntity->getNameTag()->getText());
}
