#pragma once

#include <bitset>

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"
#include "ActorFlags.h"
#include "Attribute.h"
#include "BlockLegacy.h"
#include "Components.h"
#include "Dimension.h"
#include "EntityList.h"
#include "GameType.h"
#include "Inventory.h"
#include "InventoryTransaction.h"
#include "Level.h"
#include "PlayerMovementProxy.h"
#include "TextHolder.h"
#include "EntityContext.h"
#include "../entt/src/entt/entity/registry.hpp"

// Unify all forward declarations as struct to eliminate C4099 warnings
struct GameMode;
class Level;

class Player; // Player is now a class
class Dimension;
struct MobEffect;
struct MobEffectInstance;
struct Packet;
class BlockActor;

struct EntityLocation {
    Vec3 pos;
    Vec3 posPrev;
    Vec3 velocity;
};

#pragma pack(push, 4)
struct Entity {
public:
    // Memory overlay pattern: ctx is never constructed, only overlaid from game memory
    // Using union to bypass reference member initialization requirement
    union {
        EntityContext ctx;
        char ctx_storage[sizeof(EntityContext)];
    };

    // Constructor only needed for inheritance - never actually constructs EntityContext
    Entity() : ctx_storage{} {}
    
    BUILD_ACCESS(this, __int64 **, entityRegistryBase, 0x8);
    BUILD_ACCESS(this, uint32_t, entityId, 0x10);
    BUILD_ACCESS(this, int16_t, damageTime, 0x188);
    BUILD_ACCESS(this, int, ticksAlive, 0x200);
    BUILD_ACCESS(this, Dimension *, dimension, 0x250);
    BUILD_ACCESS(this, Level *, level, 0x260);
    BUILD_ACCESS(this, EntityLocation *, entityLocation, 0x2A0);
    BUILD_ACCESS(this, int, deviceIdentifier, 0x848);

    virtual int getStatusFlag(ActorFlags);
    virtual void setStatusFlag(ActorFlags, bool);
    virtual int getLastHurtByEntity(void);
    virtual void setLastHurtByEntity(Entity *);
    virtual int getLastHurtByPlayer(void);
    virtual void setLastHurtByPlayer(Player *);
    virtual int getLastHurtEntity(void);
    virtual void setLastHurtEntity(Entity *);
    virtual void outOfWorld(void);
    virtual void reloadHardcoded(__int64, __int64 const &);
    virtual void reloadHardcodedClient(__int64, __int64 const &);
    virtual void initializeComponents(__int64, __int64 const &);
    virtual void reloadComponents(__int64, __int64 const &);
    virtual void _serverInitItemStackIds(void);
    virtual void _doInitialMove(void);
    virtual void hasComponent(HashedString const &);

private:
    virtual void Destructor();

public:
    virtual void resetUserPos(bool);
    virtual int getOwnerEntityType(void);
    virtual void remove(void);
    virtual bool isRuntimePredictedMovementEnabled(void);
    virtual int getPredictedMovementValues(void);
    virtual Vec3 *getPos(void);
    virtual Vec3 *getPosOld(void);
    virtual float getPosExtrapolated(float);
    virtual float getAttachPos(__int64, float);
    virtual int getFiringPos(void);
    virtual void move(Vec3 const &);
    virtual void move(__int64 &, Vec3 const &);
    virtual float getInterpolatedRidingPosition(float);
    virtual float getInterpolatedBodyRot(float);
    virtual float getInterpolatedHeadRot(float);
    virtual float getInterpolatedBodyYaw(float);
    virtual int getYawSpeedInDegreesPerSecond(void);
    virtual float getInterpolatedWalkAnimSpeed(float);
    virtual float getInterpolatedRidingOffset(float, int);
    virtual void resetInterpolated(void);
    virtual void updateEntityInside(AABB const &);
    virtual void updateEntityInside(void);
    virtual bool isFireImmune(void);
    virtual void breaksFallingBlocks(void);
    virtual void blockedByShield(__int64 const &, Entity &);
    virtual void canDisableShield(void);
    virtual void teleportTo(Vec3 const &, bool, int, int, bool);
    virtual void tryTeleportTo(Vec3 const &, bool, bool, int, int);
    virtual void chorusFruitTeleport(Vec3 const &);
    virtual void lerpMotion(Vec3 const &);
    virtual void tryCreateAddEntityPacket(void);
    virtual void normalTick(void);
    virtual void baseTick(void);
    virtual void passengerTick(void);
    virtual void positionPassenger(Entity &, float);
    virtual void startRiding(Entity &);
    virtual void addPassenger(Entity &);
    virtual void flagPassengerToRemove(Entity &);
    virtual int getExitTip(std::string const &, __int64, __int64);
    virtual int getEntityLocNameString(void);
    virtual void intersects(Vec3 const &, Vec3 const &);
    virtual bool isInWall(void);
    virtual bool isInvisible(void);
    virtual bool canShowNameTag(void);
    virtual void canExistInPeaceful(void);
    virtual void setNameTagVisible(bool);
    virtual TextHolder *getNameTag(void);
    virtual int getNameTagAsHash(void);
    virtual TextHolder *getFormattedNameTag(void);
    virtual void filterFormattedNameTag(__int64 const &);
    virtual void setNameTag(std::string const &);
    virtual int getAlwaysShowNameTag(void);
    virtual void setScoreTag(std::string const &);
    virtual int getScoreTag(void);
    virtual bool isInWater(void);
    virtual bool isUnderLiquid(__int64);
    virtual bool isOverWater(void);
    virtual void setBlockMovementSlowdownMultiplier(BlockLegacy const &, Vec3 const &);
    virtual void resetBlockMovementSlowdownMultiplier(void);
    virtual int getCameraOffset(void);
    virtual int getShadowHeightOffs(void);
    virtual int getShadowRadius(void);
    virtual float getHeadLookVector(float);
    virtual void canSeeInvisible(void);
    virtual bool canSee(Vec3 const &);
    virtual bool canSee(Entity const &);
    virtual void canInteractWithOtherEntitiesInGame(void);
    virtual bool isSkyLit(float);
    virtual float getBrightness(float, __int64 const &);
    virtual void interactPreventDefault(void);
    virtual void playerTouch(Player &);
    virtual void onAboveBubbleColumn(bool);
    virtual void onInsideBubbleColumn(bool);
    virtual bool isImmobile(void);
    virtual bool isSilent(void);
    virtual bool isSilentObserver(void);
    virtual bool isPickable(void);
    virtual bool isFishable(void);
    virtual bool isSleeping(void);
    virtual void setSleeping(bool);
    virtual bool isShootable(void);
    virtual void setSneaking(bool);
    virtual bool isBlocking(void);
    virtual bool isDamageBlocked(__int64 const &);
    virtual bool isAlive(void);
    virtual bool isOnFire(void);
    virtual bool isOnHotBlock(void);
    virtual bool isCreativeModeAllowed(void);
    virtual bool isSurfaceEntity(void);
    virtual bool isTargetable(void);
    virtual bool isLocalPlayer(void);
    virtual bool isRemotePlayer(void);
    virtual bool isPlayer(void);
    virtual bool isAffectedByWaterBottle(void);
    virtual bool canAttack(Entity *, bool);
    virtual void setTarget(Entity *);
    virtual bool isValidTarget(Entity *);
    virtual void attack(Entity &, __int64 const &);
    virtual void performRangedAttack(Entity &, float);
    virtual int getEquipmentCount(void);
    virtual void setOwner(uint64_t);
    virtual void setSitting(bool);
    virtual void onTame(void);
    virtual void onFailedTame(void);
    virtual int getInventorySize(void);
    virtual int getEquipSlots(void);
    virtual int getChestSlots(void);
    virtual void setStanding(bool);
    virtual void canPowerJump(void);
    virtual void setCanPowerJump(bool);
    virtual bool isEnchanted(void);
    virtual void vehicleLanded(Vec3 const &, Vec3 const &);
    virtual void shouldRender(void);
    virtual void playAmbientSound(void);
    virtual int getAmbientSound(void);
    virtual bool isInvulnerableTo(__int64 const &);
    virtual int getBlockDamageCause(Block const &);
    virtual void animateHurt(void);
    virtual void doFireHurt(int);
    virtual void onLightningHit(void);
    virtual void onBounceStarted(Vec3i const &, Block const &);
    virtual void feed(int);
    virtual void handleEntityEvent(__int64, int);
    virtual int getPickRadius(void);
    virtual int getEntityRendererId(void);
    virtual void spawnAtLocation(int, int);
    virtual void spawnAtLocation(int, int, float);
    virtual void spawnAtLocation(ItemStack const &, float);
    virtual void despawn(void);
    virtual void killed(Entity &);
    virtual void awardKillScore(Entity &, int);
    virtual void setArmor(int, ItemStack const &);
    virtual ItemStack *getArmor(int);
    virtual int getAllArmor(void);
    virtual int getArmorMaterialTypeInSlot(int);
    virtual int getArmorMaterialTextureTypeInSlot(int);
    virtual int getArmorColorInSlot(int, int);
    virtual int getEquippedSlot(int);
    virtual void setEquippedSlot(int, ItemStack const &);
    virtual void setCarriedItem(ItemStack const *);
    virtual int getCarriedItem(void);
    virtual void setOffhandSlot(ItemStack const *);
    virtual ItemStack *getEquippedTotem(void);
    virtual void consumeTotem(void);
    virtual void save(CompoundTag *);
    virtual void saveWithoutId(CompoundTag &);
    virtual void load(CompoundTag const &, __int64 &);
    virtual void loadLinks(CompoundTag const &, std::vector<__int64> &, __int64 &);
    virtual int getEntityTypeId(void);
    virtual void queryEntityRenderer(void);
    virtual int getSourceUniqueID(void);
    virtual void thawFreezeEffect(void);
    virtual void canFreeze(void);
    virtual bool isWearingLeatherArmor(void);
    virtual int getLiquidAABB(__int64);
    virtual void handleInsidePortal(Vec3i const &);
    virtual int getPortalCooldown(void);
    virtual int getPortalWaitTime(void);
    virtual void canChangeDimensionsUsingPortal(void);
    virtual void changeDimension(Dimension *, int);
    virtual void changeDimension(__int64 const &);
    virtual int getControllingPlayer(void);
    virtual void checkFallDamage(float, bool);
    virtual void causeFallDamage(float, float, __int64);
    virtual void handleFallDistanceOnServer(float, float, bool);
    virtual void playSynchronizedSound(__int64, Vec3 const &, Block const &, bool);
    virtual void playSynchronizedSound(__int64, Vec3 const &, int, bool);
    virtual void onSynchedFlagUpdate(int, long, long);
    virtual void onSynchedDataUpdate(int);
    virtual void canAddPassenger(Entity &);
    virtual void canPickupItem(ItemStack const &);
    virtual void canBePulledIntoVehicle(void);
    virtual void inCaravan(void);
    virtual bool isLeashableType(void);
    virtual void tickLeash(void);
    virtual void sendMotionPacketIfNeeded(void);
    virtual void canSynchronizeNewEntity(void);
    virtual void stopRiding(bool, bool, bool);
    virtual void startSwimming(void);
    virtual void stopSwimming(void);
    virtual void buildDebugInfo(std::string &);
    virtual int getCommandPermissionLevel(void);
    virtual bool isClientSide(void);
    virtual class AttributeInstance *getMutableAttribute(class Attribute *Attribute);
    virtual class AttributeInstance *getAttribute(class Attribute *Attribute);
    virtual int getDeathTime(void);
    virtual void heal(int);
    virtual bool isInvertedHealAndHarm(void);
    virtual void canBeAffected(int);
    virtual void canBeAffectedByArrow(MobEffectInstance const &);
    virtual void onEffectAdded(MobEffectInstance &);
    virtual void onEffectUpdated(MobEffectInstance &);
    virtual void onEffectRemoved(MobEffectInstance &);
    virtual void canObstructSpawningAndBlockPlacement(void);
    virtual class AnimationComponent *getAnimationComponent(void);
    virtual void openContainerComponent(Player &);
    virtual void swing(void);
    virtual void useItem(ItemStack &, __int64, bool);
    virtual void hasOutputSignal(char);
    virtual int getOutputSignal(void);
    virtual int getDebugText(std::vector<std::string> &);
    virtual int getMapDecorationRotation(void);
    virtual int getPassengerYRotation(Entity const &);
    virtual void add(ItemStack &);
    virtual void drop(ItemStack const &, bool);
    virtual int getInteraction(Player &, __int64 &, Vec3 const &);
    virtual void canDestroyBlock(Block const &);
    virtual void setAuxValue(int);
    virtual void setSize(float, float);
    virtual void onOrphan(void);
    virtual void wobble(void);
    virtual void wasHurt(void);
    virtual void startSpinAttack(void);
    virtual void stopSpinAttack(void);
    virtual void setDamageNearbyEntitys(bool);
    virtual void renderDebugServerState(__int64 const &);
    virtual void reloadLootTable(void);
    virtual void reloadLootTable(__int64 const &);
    virtual int getDeletionDelayTimeSeconds(void);
    virtual void kill(void);
    virtual void die(__int64 const &);
    virtual void shouldDropDeathLoot(void);
    virtual void shouldTick(void);
    virtual void applySnapshot(__int64 const &, __int64 const &);
    virtual float getNextStep(float);
    virtual int getLootTable(void);
    virtual void onPush(Entity &);
    virtual Vec3i getLastDeathPos(void);
    virtual int getLastDeathDimension(void);
    virtual void hasDiedBefore(void);
    virtual void doWaterSplashEffect(void);
    virtual void _shouldProvideFeedbackOnHandContainerItemSet(int, ItemStack const &);
    virtual void _shouldProvideFeedbackOnArmorSet(int, ItemStack const &);
    virtual void updateEntitySpecificMolangVariables(__int64 &);
    virtual void shouldTryMakeStepSound(void);
    virtual void canMakeStepSound(void);
    virtual void _hurt(__int64 const &, float, bool, bool);
    virtual void markHurt(void);
    virtual void _getAnimationComponent(__int64 &, __int64);
    virtual void readAdditionalSaveData(CompoundTag const &, __int64 &);
    virtual void addAdditionalSaveData(CompoundTag &);
    virtual void _playStepSound(Vec3i const &, Block const &);
    virtual void _playFlySound(Vec3i const &, Block const &);
    virtual void _makeFlySound(void);
    virtual void checkInsideBlocks(float);
    virtual void pushOutOfBlocks(Vec3 const &);
    virtual void spawnTrailBubbles(void);
    virtual void updateInsideBlock(void);
    virtual void _removePassenger(uint64_t const &, bool, bool, bool);
    virtual void _onSizeUpdated(void);
    virtual void _doAutoAttackOnTouch(Entity &);
    virtual void knockback(Entity *, int, float, float, float, float, float);
    virtual void spawnAnim(void);
    virtual void setSprinting(bool);
    virtual int getHurtSound(void);
    virtual int getDeathSound(void);
    virtual __int64 getSpeed(void);
    virtual void setSpeed(float);
    virtual void hurtEffects(__int64 const &, float, bool, bool);
    virtual int getMeleeWeaponDamageBonus(Entity);
    virtual int getMeleeKnockbackBonus(void);
    virtual void travel(float, float, float, bool);
    virtual void applyFinalFriction(float, bool);
    virtual void aiStep(void);
    virtual void aiStep(__int64 &);
    virtual void pushEntitys(void);
    virtual void lookAt(Entity *, float, float);
    virtual bool isLookingAtAnEntity(void);
    virtual void checkSpawnRules(bool);
    virtual void checkSpawnObstruction(void);
    virtual float getAttackAnim(float);
    virtual int getItemUseDuration(void);
    virtual int getItemUseStartupProgress(void);
    virtual int getItemUseIntervalProgress(void);
    virtual int getItemUseIntervalAxis(void);
    virtual int getTimeAlongSwing(void);
    virtual void ate(void);
    virtual int getMaxHeadXRot(void);
    virtual bool isAlliedTo(Entity *);
    virtual void doHurtTarget(Entity *, __int64 const &);
    virtual void canBeControlledByPassenger(void);
    virtual void leaveCaravan(void);
    virtual void joinCaravan(Entity);
    virtual void hasCaravanTail(void);
    virtual int getCaravanHead(void);
    virtual int getArmorValue(void);
    virtual int getArmorCoverPercentage(void);
    virtual int getToughnessValue(void);
    virtual void hurtArmorSlots(__int64 const &, int, std::bitset<4ul>);
    virtual void setDamagedArmor(int, ItemStack const &);
    virtual void sendArmorDamage(std::bitset<4ul>);
    virtual void sendArmor(std::bitset<4ul>);
    virtual void containerChanged(int);
    virtual void updateEquipment(void);
    virtual void clearEquipment(void);
    virtual int getAllArmorID(void);
    virtual int getAllHand(void);
    virtual int getAllEquipment(void);
    virtual int getArmorTypeHash(void);
    virtual void dropEquipmentOnDeath(__int64 const &, int);
    virtual void dropEquipmentOnDeath(void);
    virtual void clearVanishEnchantedItemsOnDeath(void);
    virtual void sendInventory(bool shouldSelectSlot);
    virtual float getDamageAfterEnchantReduction(__int64 const &, float);
    virtual float getDamageAfterArmorReduction(__int64 const &, float);
    virtual float getDamageAfterResistanceEffect(__int64 const &, float);
    virtual void createAIGoals(void);
    virtual void onBorn(Entity &, Entity &);
    virtual void setItemSlot(int, ItemStack const &);
    virtual void setTransitioningSitting(bool);
    virtual void attackAnimation(Entity *, float);
    virtual int getAttackTime(void);
    virtual void _getWalkTargetValue(Vec3i const &);
    virtual void canExistWhenDisallowEntity(void);
    virtual void ascendLadder(void);
    virtual void ascendBlockByJumping(void);
    virtual void descendBlockByCrouching(void);
    virtual void dropContainer(void);
    virtual void initBodyControl(void);
    virtual void jumpFromGround(BlockSource const &);
    virtual void jumpFromGround(PlayerMovementProxy &, BlockSource const &);
    virtual void newServerAiStep(void);
    virtual void _serverAiEntityStep(void);
    virtual void dropBags(void);
    virtual void tickDeath(void);
    virtual void updateGliding(void);
    virtual void _allowAscendingScaffolding(void);
    virtual void _getAdjustedAABBForSpawnCheck(AABB const &, Vec3 const &);

public:
    InventoryTransactionManager *getTransactionManager();

    __int64 *getUniqueId() {
        using getUniqueId_t = __int64*(__thiscall *)(Entity*);
        static auto getUniqueIdFunc = reinterpret_cast<getUniqueId_t>(FindSignature("40 53 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B 51 ? 48 8B D9 8B 41"));
        return getUniqueIdFunc(this);
    }

    float getRealSpeed() {
        return *reinterpret_cast<float *>(this->getSpeed() + 0x84);
    }

    float getTicksPerSecond() {
        Vec3 targetPos = *this->getPos();
        Vec3 targetPosOld = *this->getPosOld();
        float hVel = sqrtf(((targetPos.x - targetPosOld.x) * (targetPos.x - targetPosOld.x)) + ((targetPos.z - targetPosOld.z) * (targetPos.z - targetPosOld.z)));
        return hVel;
    }

    float getBlocksPerSecond();

    int getTicksUsingItem() {
        return *reinterpret_cast<int *>(this + 0xFF8);
    }

    bool isSneaking() {
        return getStatusFlag(SNEAKING);
    }

    bool isSprinting() {
        return getStatusFlag(SPRINTING);
    }

    Dimension *getDimension() {
        return this->dimension;
    }

    void setIsInWater(bool is) {
        *(bool *)((uintptr_t)(this) + 0x1FE) = is;
    }

    void SetFieldOfView(float num) {
        *(float *)((uintptr_t)(this) + 0x1048) = num;
    }

    float GetFieldOfView() {
        return *reinterpret_cast<float *>(this + 0x1048);
    }

    Level *getLevel() {
        return this->level;
    }

    PlayerMovementProxy *getMovementProxy() {
        return getActorMovementProxyComponent()->movementProxy;
    }

    bool isOnGround() {
        return getMovementProxy()->isOnGround();
    }

    void setOnGround(bool onGround) {
        getMovementProxy()->setOnGround(onGround);
    }

    void setPos(Vec3 vec) {
        getMovementProxy()->setPos(vec);
    }

    BlockSource* getRegion() {
        return getBlockSourceComponent()->region;
    }

    void jumpFromGround() {
        this->jumpFromGround(*getRegion());
    }

    void lerpTo(Vec3 const &pos, Vec2 const &rot, int steps);

    ActorRotationComponent *getActorRotationComponent() {
        using getActorRotationComponent = ActorRotationComponent *(__cdecl *)(__int64 *, uint32_t *);
        static auto func = reinterpret_cast<getActorRotationComponent>(FindSignature("40 53 48 83 EC ? 48 8B DA BA CE 21 1E DC"));
        uint32_t id = this->entityId;
        return func(*this->entityRegistryBase, &id);
    }

    MobBodyRotationComponent *getMobBodyRotationComponent() {
        using getMobBodyRotationComponent = MobBodyRotationComponent *(__cdecl *)(__int64 *, uint32_t *);
        static auto func = reinterpret_cast<getMobBodyRotationComponent>(FindSignature("40 53 48 83 EC ? 48 8B DA BA 2F B8 31 03"));
        uint32_t id = this->entityId;
        return func(*this->entityRegistryBase, &id);
    }

    ActorHeadRotationComponent *getActorHeadRotationComponent() {
        using getActorHeadRotationComponent = ActorHeadRotationComponent *(__cdecl *)(__int64 *, uint32_t *);
        static auto func = reinterpret_cast<getActorHeadRotationComponent>(FindSignature("40 53 48 83 EC ? 48 8B DA BA 1C 58 40 E9"));
        uint32_t id = this->entityId;
        return func(*this->entityRegistryBase, &id);
    }

    MaxAutoStepComponent *getMaxAutoStepComponent() {
        using getMaxAutoStep = MaxAutoStepComponent *(__cdecl *)(__int64 *, uint32_t *);
        static auto func = reinterpret_cast<getMaxAutoStep>(FindSignature("40 53 48 83 EC ? 48 8B DA BA 16 72 6F 0E"));
        uint32_t id = this->entityId;
        return func(*this->entityRegistryBase, &id);
    }

    FallDistanceComponent *getFallDistanceComponent() {
        using getFallDistanceComponent = FallDistanceComponent *(__cdecl *)(__int64 *, uint32_t *);
        static auto func = reinterpret_cast<getFallDistanceComponent>(FindSignature("40 53 48 83 EC ? 48 8B DA BA B5 5C 12 81"));
        uint32_t id = this->entityId;
        return func(*this->entityRegistryBase, &id);
    }

    ActorMovementProxyComponent *getActorMovementProxyComponent() {
        using getActorMovementProxyComponent = ActorMovementProxyComponent *(__cdecl *)(__int64 *, uint32_t *);
        static auto func = reinterpret_cast<getActorMovementProxyComponent>(FindSignature("40 53 48 83 EC ? 48 8B DA BA 18 0C BD EC"));
        uint32_t id = this->entityId;
        return func(*this->entityRegistryBase, &id);
    }

    BlockSourceComponent *getBlockSourceComponent() {
        using getBlockSourceComponent = BlockSourceComponent *(__cdecl *)(__int64 *, uint32_t *);
        static auto func = reinterpret_cast<getBlockSourceComponent>(FindSignature("40 53 48 83 EC ? 48 8B DA BA 32 47 C1 FD"));
        uint32_t id = this->entityId;
        return func(*this->entityRegistryBase, &id);
    }

    RuntimeIDComponent *getRuntimeIDComponent() {
        using getRuntimeIDComponent = RuntimeIDComponent *(__cdecl *)(__int64 *, uint32_t *);
        static auto func = reinterpret_cast<getRuntimeIDComponent>(FindSignature("40 53 48 83 EC ? 48 8B DA BA 14 14 A1 3C"));
        uint32_t id = this->entityId;
        return func(*this->entityRegistryBase, &id);
    }

    ActorGameTypeComponent *getActorGameTypeComponent() {
        using getActorGameTypeComponent = ActorGameTypeComponent *(__cdecl *)(__int64 *, uint32_t *);
        static auto func = reinterpret_cast<getActorGameTypeComponent>(FindSignature("40 53 48 83 EC ? 48 8B DA BA DE AB CB AF"));
        uint32_t id = this->entityId;
        return func(*this->entityRegistryBase, &id);
    }

    RenderPositionComponent *getRenderPositionComponent() {
        using getRenderPositionComponent = RenderPositionComponent *(__cdecl *)(void *, uint32_t *);
        static auto func = reinterpret_cast<getRenderPositionComponent>(FindSignature("40 53 48 83 EC ? 48 8B DA BA 6E F3 E8 D4"));
        uint32_t id = this->entityId;
        return func(*this->entityRegistryBase, &id);
    }

    AABBShapeComponent *getAABBShapeComponent() {
        using getAABBShapeComponent = AABBShapeComponent *(__cdecl *)(void *, uint32_t *);
        static auto func = reinterpret_cast<getAABBShapeComponent>(FindSignature("40 53 48 83 EC ? 48 8B DA BA F2 C9 10 1B"));
        uint32_t id = this->entityId;
        return func(*this->entityRegistryBase, &id);
    }
};
#pragma pack(pop)

struct MobEffect {
public:
    int id; // 0x8
    bool isHarmful; // 0xC

private:
    char pad_0xD[0x13]; // 0xD

public:
    TextHolder* internalName; // 0x20

private:
    char pad_0x28[0x28]; // 0x28

public:
    TextHolder name; // 0x50

private:
    char pad_0x70[0x30]; // 0x70

public:
    TextHolder* minecraftName; // 0xA0

private:
    char pad_0xA8[0xD0];  // 0xA8

private:
    virtual void destructor();

public:
    virtual void applyEffects(Entity *, int, int);
    virtual void removeEffects(Entity *);
    virtual void applyInstantaneousEffect(Entity *, Entity *, Entity *, int, float);
    virtual void parse(void *, __int64 const &, __int64 const &, int, std::string &);
    virtual int getAttributeModifierValue(int, __int64 const &);
};


// Forward declarations - actual implementations in Player.h and LocalPlayer.h
class Player;
class ServerPlayer;
class LocalPlayer;
