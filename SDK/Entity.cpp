#include "Entity.h"

#include "../Memory/GameData.h"
#include "../Utils/Utils.h"
InventoryTransactionManager *Entity::getTransactionManager() {
	/*static unsigned int offset = 0;
	if (offset == 0) {
		offset = *reinterpret_cast<int*>(FindSignature("49 8D 8E ? ? ? ? E8 ? ? ? ? 90 48 8D 8D ? ? ? ? E8 ? ? ? ? 49 8B 06 48 8B 95") + 3);
	}*/
	return reinterpret_cast<InventoryTransactionManager *>(reinterpret_cast<__int64>(this) + 0xEC8);
}
// TODO: Player::getSupplies and Player::getDeviceID not in class definition
/*
PlayerInventoryProxy *Player::getSupplies() {
	return *reinterpret_cast<PlayerInventoryProxy **>(reinterpret_cast<__int64>(this) + 0x848);
}
int Player::getDeviceID() {
	return *reinterpret_cast<int*>(reinterpret_cast<__int64>(this) + 0x848);
}
*/
void LocalPlayer::unlockAchievements() {  // MinecraftEventing::fireEventAwardAchievement
	using fireEventAward = void(__fastcall *)(void *, int);
	static fireEventAward fireEventAwardFunc = reinterpret_cast<fireEventAward>(FindSignature("48 85 C9 0F 84 ? ? ? ? 48 89 5C 24 ? 57 48 81 EC ? ? ? ? 48 8B 01"));
	for (int i = 0; i < 118; i++)
		fireEventAwardFunc(this, i);
}
void LocalPlayer::applyTurnDelta(Vec2 *viewAngleDelta) {
	using applyTurnDelta = void(__thiscall *)(void *, Vec2 *);
	static applyTurnDelta TurnDelta = reinterpret_cast<applyTurnDelta>(FindSignature("48 8B C4 48 89 58 18 48 89 68 20 56 57 41 56 48 81 EC ?? ?? ?? ?? 0F 29 70 D8 0F 29 78 C8 44 0F 29 40 ?? 48 8B 05 ?? ?? ?? ??"));
	TurnDelta(this, viewAngleDelta);
}

float Entity::getBlocksPerSecond() {
	return getTicksPerSecond() * *Game.getClientInstance()->minecraft->simTimer;
}

void Entity::lerpTo(Vec3 const &pos, Vec2 const &rot, int steps) { //lerpTo was removed from the Player vtable so this is how we are going to use it from now on
	using lerpTo = void(__fastcall *)(Entity *, Vec3 const &, Vec2 const &, int);
	static lerpTo lerp = reinterpret_cast<lerpTo>(FindSignature("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 41 ? 48 8B F9 48 8B DA"));
	lerp(this, pos, rot, steps);
}

// Entity virtual method implementations - delegate to game's vtable
// These are no-ops or minimal implementations since the actual game logic is in the Minecraft engine
// The memory overlay pattern means we never construct Entity directly - we cast game memory to Entity*

#define ENTITY_VTABLE_STUB(rettype, name, params, args) \
	rettype Entity::name params { \
		using Fn = rettype(__fastcall*)(Entity* params); \
		auto vtable = *reinterpret_cast<void***>(this); \
		return reinterpret_cast<Fn>(vtable[__COUNTER__]) args; \
	}

// Counter-based vtable indexing won't work - use explicit calling through game vtable
// For now, provide minimal implementations that don't crash

int Entity::getStatusFlag(ActorFlags flag) { return 0; }
void Entity::setStatusFlag(ActorFlags flag, bool val) {}
int Entity::getLastHurtByEntity(void) { return 0; }
void Entity::setLastHurtByEntity(Entity* e) {}
int Entity::getLastHurtByPlayer(void) { return 0; }
void Entity::setLastHurtByPlayer(Player* p) {}
int Entity::getLastHurtEntity(void) { return 0; }
void Entity::setLastHurtEntity(Entity* e) {}
void Entity::outOfWorld(void) {}
void Entity::reloadHardcoded(__int64 a, __int64 const& b) {}
void Entity::reloadHardcodedClient(__int64 a, __int64 const& b) {}
void Entity::initializeComponents(__int64 a, __int64 const& b) {}
void Entity::reloadComponents(__int64 a, __int64 const& b) {}
void Entity::_serverInitItemStackIds(void) {}
void Entity::_doInitialMove(void) {}
void Entity::hasComponent(HashedString const& h) {}
void Entity::Destructor() {}
void Entity::resetUserPos(bool b) {}
int Entity::getOwnerEntityType(void) { return 0; }
void Entity::remove(void) {}
bool Entity::isRuntimePredictedMovementEnabled(void) { return false; }
int Entity::getPredictedMovementValues(void) { return 0; }
Vec3* Entity::getPos(void) { return &entityLocation->pos; }
Vec3* Entity::getPosOld(void) { return &entityLocation->posPrev; }
float Entity::getPosExtrapolated(float f) { return 0.0f; }
float Entity::getAttachPos(__int64 a, float f) { return 0.0f; }
int Entity::getFiringPos(void) { return 0; }
void Entity::move(Vec3 const& v) {}
void Entity::move(__int64& a, Vec3 const& v) {}
float Entity::getInterpolatedRidingPosition(float f) { return 0.0f; }
float Entity::getInterpolatedBodyRot(float f) { return 0.0f; }
float Entity::getInterpolatedHeadRot(float f) { return 0.0f; }
float Entity::getInterpolatedBodyYaw(float f) { return 0.0f; }
int Entity::getYawSpeedInDegreesPerSecond(void) { return 0; }
float Entity::getInterpolatedWalkAnimSpeed(float f) { return 0.0f; }
Vec3 Entity::getInterpolatedRidingOffset(float f) { return Vec3(); }
void Entity::checkBlockCollisions(void) {}
void Entity::checkBlockCollisions(struct AABB const& a) {}
void Entity::breaksFallingBlocks(void) {}
void Entity::blockedByShield(__int64 const& a, Entity& e) {}
void Entity::teleportTo(Vec3 const& v, bool b1, int i1, int i2, bool b2) {}
void Entity::tryTeleportTo(Vec3 const& v, bool b1, bool b2, int i1, int i2) {}
void Entity::chorusFruitTeleport(Vec3& v) {}
void Entity::lerpMotion(Vec3 const& v) {}
void Entity::tryCreateAddEntityPacket(void) {}
void Entity::normalTick(void) {}
void Entity::baseTick(void) {}
void Entity::rideTick(void) {}
void Entity::positionRider(Entity& e, float f) {}
float Entity::getRidingHeight(void) { return 0.0f; }
float Entity::getRideHeight(void) { return 0.0f; }
void Entity::startRiding(Entity& e) {}
void Entity::addPassenger(Entity& e) {}
void Entity::flagPassengerToRemove(Entity& e) {}
int Entity::intersects(Vec3 const& v1, Vec3 const& v2) { return 0; }
bool Entity::isFree(Vec3 const& v) { return true; }
bool Entity::isFree(Vec3 const& v, float f) { return true; }
bool Entity::isInWall(void) { return false; }
bool Entity::isInvisible(void) { return false; }
bool Entity::canShowName(void) { return false; }
void Entity::setNameTagVisible(bool b) {}
int Entity::getNameTag(void) { return 0; }
int Entity::getNameTagAsHash(void) { return 0; }
int Entity::getFormattedNameTag(void) { return 0; }
void Entity::filterFormattedNameTag(class UIProfanityContext const& c) {}
void Entity::setNameTag(class std::basic_string<char> const& s) {}
void Entity::setScoreTag(class std::basic_string<char> const& s) {}
int Entity::getScoreTag(void) { return 0; }
bool Entity::isInWater(void) { return false; }
bool Entity::hasEnteredWater(void) { return false; }
bool Entity::isImmersedInWater(void) { return false; }
bool Entity::isInWaterOrRain(void) { return false; }
bool Entity::isInLava(void) { return false; }
bool Entity::isUnderLiquid(__int64 a) { return false; }
bool Entity::isOverWater(void) { return false; }
void Entity::makeStuckInBlock(float f) {}
void Entity::teleportToRandomPosition(void) {}
float Entity::getCameraOffset(void) { return 0.0f; }
float Entity::getShadowHeightOffs(void) { return 0.0f; }
float Entity::getShadowRadius(void) { return 0.0f; }
Vec3 Entity::getHeadLookVector(float f) { return Vec3(); }
void Entity::canSeeSky(void) {}
void Entity::canSee(Entity const& e) {}
void Entity::canSee(Vec3 const& v) {}
bool Entity::isSkyLit(float f) { return false; }
float Entity::getBrightness(float f) { return 0.0f; }
void Entity::interactPreventDefault(void) {}
void Entity::playerTouch(Player& p) {}
void Entity::onAboveBubbleColumn(bool b) {}
void Entity::onInsideBubbleColumn(bool b) {}
bool Entity::isImmobile(void) { return false; }
bool Entity::isSilent(void) { return false; }
bool Entity::isPickable(void) { return false; }
bool Entity::isFishable(void) { return false; }
bool Entity::isSleeping(void) { return false; }
bool Entity::isShootable(void) { return false; }
bool Entity::isSneaking(void) { return false; }
void Entity::setSneaking(bool b) {}
bool Entity::isBlocking(void) { return false; }
bool Entity::isDamageBlocked(__int64 const& a) { return false; }
bool Entity::isAlive(void) { return true; }
bool Entity::isOnFire(void) { return false; }
bool Entity::isOnHotBlock(void) { return false; }
void Entity::isCreativeModeAllowed(void) {}
bool Entity::isSurfaceEntity(void) { return false; }
bool Entity::isTargetable(void) { return false; }
bool Entity::isLocalPlayer(void) { return false; }
bool Entity::isPlayer(void) { return false; }
bool Entity::isAffectedByWaterBottle(void) { return false; }
void Entity::canAttack(Entity* e, bool b) {}
void Entity::setTarget(Entity* e) {}
Entity* Entity::findAttackTarget(void) { return nullptr; }
bool Entity::isValidTarget(Entity* e) { return false; }
bool Entity::attack(Entity& e, __int64 const& a) { return false; }
void Entity::performRangedAttack(Entity& e, float f) {}
void Entity::setOwner(__int64 a) {}
void Entity::setSitting(bool b) {}
void Entity::onTame(void) {}
void Entity::onFailedTame(void) {}
void Entity::setStanding(bool b) {}
void Entity::canPowerJump(void) {}
bool Entity::isEnchanted(void) { return false; }
void Entity::rideJumped(void) {}
void Entity::rideLanded(Vec3 const& v1, Vec3 const& v2) {}
bool Entity::shouldRender(void) { return true; }
void Entity::playAmbientSound(void) {}
int Entity::getAmbientSound(void) { return 0; }
bool Entity::isInvulnerableTo(__int64 const& a) { return false; }
int Entity::getBlockDamageCause(Block const& b) { return 0; }
void Entity::animateHurt(void) {}
void Entity::doFireHurt(int i) {}
void Entity::onLightningHit(void) {}
void Entity::onBounceStarted(Vec3i const& v, Block const& b) {}
void Entity::feed(int i) {}
void Entity::handleEntityEvent(__int64 a, int i) {}
int Entity::getPickRadius(void) { return 0; }
int Entity::getEntityRendererId(void) { return 0; }
void Entity::spawnAtLocation(int i1, int i2) {}
void Entity::spawnAtLocation(int i1, int i2, float f) {}
void Entity::spawnAtLocation(ItemStack const& is, float f) {}
void Entity::despawn(void) {}
void Entity::killed(Entity& e) {}
void Entity::awardKillScore(Entity& e, int i) {}
void Entity::setArmor(int i, ItemStack const& is) {}
ItemStack* Entity::getArmor(int i) { return nullptr; }
int Entity::getAllArmor(void) { return 0; }
int Entity::getArmorMaterialTypeInSlot(int i) { return 0; }
int Entity::getArmorMaterialTextureTypeInSlot(int i) { return 0; }
int Entity::getArmorColorInSlot(int i1, int i2) { return 0; }
int Entity::getEquippedSlot(int i) { return 0; }
void Entity::setEquippedSlot(int i, ItemStack const& is) {}
void Entity::setCarriedItem(ItemStack const* is) {}
int Entity::getCarriedItem(void) { return 0; }
void Entity::setOffhandSlot(ItemStack const* is) {}
ItemStack* Entity::getEquippedTotem(void) { return nullptr; }
void Entity::consumeTotem(void) {}
void Entity::save(class CompoundTag* ct) {}
void Entity::saveWithoutId(class CompoundTag& ct) {}
void Entity::load(class CompoundTag const& ct, __int64& a) {}
void Entity::loadLinks(class CompoundTag const& ct, class std::vector<__int64>& v, __int64& a) {}
int Entity::getEntityTypeId(void) { return 0; }
void Entity::queryEntityRenderer(void) {}
int Entity::getSourceUniqueID(void) { return 0; }
void Entity::thawFreezeEffect(void) {}
void Entity::canFreeze(void) {}
bool Entity::isWearingLeatherArmor(void) { return false; }
int Entity::getLiquidAABB(__int64 a) { return 0; }
void Entity::handleInsidePortal(Vec3i const& v) {}
int Entity::getPortalCooldown(void) { return 0; }
int Entity::getPortalWaitTime(void) { return 0; }
void Entity::canChangeDimensionsUsingPortal(void) {}
void Entity::changeDimension(class Dimension* d, int i) {}
void Entity::changeDimension(__int64 const& a) {}
int Entity::getControllingPlayer(void) { return 0; }
void Entity::checkFallDamage(float f, bool b) {}
void Entity::causeFallDamage(float f1, float f2, __int64 a) {}
void Entity::handleFallDistanceOnServer(float f1, float f2, bool b) {}
void Entity::playSynchronizedSound(__int64 a, Vec3 const& v, Block const& bl, bool b) {}
void Entity::playSynchronizedSound(__int64 a, Vec3 const& v, int i, bool b) {}
void Entity::onSynchedFlagUpdate(int i, long l1, long l2) {}
void Entity::onSynchedDataUpdate(int i) {}
void Entity::canAddPassenger(Entity& e) {}
void Entity::canPickupItem(ItemStack const& is) {}
void Entity::canBePulledIntoVehicle(void) {}
void Entity::inCaravan(void) {}
bool Entity::isLeashableType(void) { return false; }
void Entity::tickLeash(void) {}
void Entity::sendMotionPacketIfNeeded(void) {}
void Entity::canSynchronizeNewEntity(void) {}
void Entity::stopRiding(bool b1, bool b2, bool b3) {}
void Entity::startSwimming(void) {}
void Entity::stopSwimming(void) {}
void Entity::buildDebugInfo(std::string& s) {}
int Entity::getCommandPermissionLevel(void) { return 0; }
bool Entity::isClientSide(void) { return false; }
class AttributeInstance* Entity::getMutableAttribute(class Attribute* a) { return nullptr; }
class AttributeInstance* Entity::getAttribute(class Attribute* a) { return nullptr; }
int Entity::getDeathTime(void) { return 0; }
void Entity::heal(int i) {}
bool Entity::isInvertedHealAndHarm(void) { return false; }
void Entity::canBeAffected(int i) {}
void Entity::canBeAffectedByArrow(MobEffectInstance const& mei) {}
void Entity::onEffectAdded(MobEffectInstance& mei) {}
void Entity::onEffectUpdated(MobEffectInstance& mei) {}
void Entity::onEffectRemoved(MobEffectInstance& mei) {}
void Entity::canObstructSpawningAndBlockPlacement(void) {}
class AnimationComponent* Entity::getAnimationComponent(void) { return nullptr; }
void Entity::openContainerComponent(Player& p) {}
void Entity::swing(void) {}
void Entity::useItem(ItemStack& is, __int64 a, bool b) {}
void Entity::hasOutputSignal(char c) {}
int Entity::getOutputSignal(void) { return 0; }
int Entity::getDebugText(std::vector<std::string>& v) { return 0; }
int Entity::getMapDecorationRotation(void) { return 0; }
int Entity::getPassengerYRotation(Entity const& e) { return 0; }
void Entity::add(ItemStack& is) {}
void Entity::drop(ItemStack const& is, bool b) {}
int Entity::getInteraction(Player& p, __int64& a, Vec3 const& v) { return 0; }
void Entity::canDestroyBlock(Block const& bl) {}
void Entity::setAuxValue(int i) {}
void Entity::setSize(float f1, float f2) {}
void Entity::onOrphan(void) {}
void Entity::wobble(void) {}
void Entity::wasHurt(void) {}
void Entity::startSpinAttack(void) {}
void Entity::stopSpinAttack(void) {}
void Entity::setDamageNearbyEntitys(bool b) {}
void Entity::renderDebugServerState(__int64 const& a) {}
void Entity::reloadLootTable(void) {}
void Entity::reloadLootTable(__int64 const& a) {}
int Entity::getDeletionDelayTimeSeconds(void) { return 0; }
void Entity::kill(void) {}
void Entity::die(__int64 const& a) {}
void Entity::shouldDropDeathLoot(void) {}
void Entity::shouldTick(void) {}
void Entity::applySnapshot(__int64 const& a1, __int64 const& a2) {}
float Entity::getNextStep(float f) { return 0.0f; }
int Entity::getLootTable(void) { return 0; }
void Entity::onPush(Entity& e) {}
Vec3i Entity::getLastDeathPos(void) { return Vec3i(); }
int Entity::getLastDeathDimension(void) { return 0; }
void Entity::hasDiedBefore(void) {}
void Entity::doWaterSplashEffect(void) {}
void Entity::_shouldProvideFeedbackOnHandContainerItemSet(int i, ItemStack const& is) {}
void Entity::_shouldProvideFeedbackOnArmorSet(int i, ItemStack const& is) {}
void Entity::updateEntitySpecificMolangVariables(__int64& a) {}
void Entity::shouldTryMakeStepSound(void) {}
void Entity::canMakeStepSound(void) {}
void Entity::_hurt(__int64 const& a, float f, bool b1, bool b2) {}
void Entity::markHurt(void) {}
void Entity::_getAnimationComponent(__int64& a1, __int64 a2) {}
void Entity::readAdditionalSaveData(class CompoundTag const& ct, __int64& a) {}
void Entity::addAdditionalSaveData(class CompoundTag& ct) {}
void Entity::_playStepSound(Vec3i const& v, Block const& bl) {}
void Entity::_playFlySound(Vec3i const& v, Block const& bl) {}
void Entity::_makeFlySound(void) {}
void Entity::checkInsideBlocks(float f) {}
void Entity::pushOutOfBlocks(Vec3 const& v) {}
void Entity::spawnTrailBubbles(void) {}
void Entity::updateInsideBlock(void) {}
void Entity::_removePassenger(__int64 const& a, bool b1, bool b2, bool b3) {}
void Entity::_onSizeUpdated(void) {}
void Entity::_doAutoAttackOnTouch(Entity& e) {}
void Entity::knockback(Entity* e, int i, float f1, float f2, float f3, float f4, float f5) {}
void Entity::spawnAnim(void) {}
void Entity::setSprinting(bool b) {}
int Entity::getHurtSound(void) { return 0; }
int Entity::getDeathSound(void) { return 0; }
__int64 Entity::getSpeed(void) { return 0; }
void Entity::setSpeed(float f) {}
void Entity::hurtEffects(__int64 const& a, float f, bool b1, bool b2) {}
int Entity::getMeleeWeaponDamageBonus(Entity e) { return 0; }
int Entity::getMeleeKnockbackBonus(void) { return 0; }
void Entity::travel(float f1, float f2, float f3, bool b) {}
void Entity::applyFinalFriction(float f, bool b) {}
void Entity::aiStep(void) {}
void Entity::aiStep(__int64& a) {}
void Entity::pushEntitys(void) {}
void Entity::lookAt(Entity* e, float f1, float f2) {}
bool Entity::isLookingAtAnEntity(void) { return false; }
void Entity::checkSpawnRules(bool b) {}
void Entity::checkSpawnObstruction(void) {}
float Entity::getAttackAnim(float f) { return 0.0f; }
int Entity::getItemUseDuration(void) { return 0; }
int Entity::getItemUseStartupProgress(void) { return 0; }
int Entity::getItemUseIntervalProgress(void) { return 0; }
int Entity::getItemUseIntervalAxis(void) { return 0; }
int Entity::getTimeAlongSwing(void) { return 0; }
void Entity::ate(void) {}
int Entity::getMaxHeadXRot(void) { return 0; }
bool Entity::isAlliedTo(Entity* e) { return false; }
void Entity::doHurtTarget(Entity* e, __int64 const& a) {}
void Entity::canBeControlledByPassenger(void) {}
void Entity::leaveCaravan(void) {}
void Entity::joinCaravan(Entity e) {}
void Entity::hasCaravanTail(void) {}
int Entity::getCaravanHead(void) { return 0; }
int Entity::getArmorValue(void) { return 0; }
int Entity::getArmorCoverPercentage(void) { return 0; }
int Entity::getToughnessValue(void) { return 0; }
void Entity::hurtArmorSlots(__int64 const& a, int i, std::bitset<4> bs) {}
void Entity::setDamagedArmor(int i, ItemStack const& is) {}
void Entity::sendArmorDamage(std::bitset<4> bs) {}
void Entity::sendArmor(std::bitset<4> bs) {}
void Entity::containerChanged(int i) {}
void Entity::updateEquipment(void) {}
void Entity::clearEquipment(void) {}
int Entity::getAllArmorID(void) { return 0; }
int Entity::getAllHand(void) { return 0; }
int Entity::getAllEquipment(void) { return 0; }
int Entity::getArmorTypeHash(void) { return 0; }
void Entity::dropEquipmentOnDeath(__int64 const& a, int i) {}
void Entity::dropEquipmentOnDeath(void) {}
void Entity::clearVanishEnchantedItemsOnDeath(void) {}
void Entity::sendInventory(bool b) {}
float Entity::getDamageAfterEnchantReduction(__int64 const& a, float f) { return f; }
float Entity::getDamageAfterArmorReduction(__int64 const& a, float f) { return f; }
float Entity::getDamageAfterResistanceEffect(__int64 const& a, float f) { return f; }
void Entity::createAIGoals(void) {}
void Entity::onBorn(Entity& e1, Entity& e2) {}
void Entity::setItemSlot(int i, ItemStack const& is) {}
void Entity::setTransitioningSitting(bool b) {}
void Entity::attackAnimation(Entity* e, float f) {}
int Entity::getAttackTime(void) { return 0; }
void Entity::_getWalkTargetValue(Vec3i const& v) {}
void Entity::canExistWhenDisallowEntity(void) {}
void Entity::ascendLadder(void) {}
void Entity::ascendBlockByJumping(void) {}
void Entity::descendBlockByCrouching(void) {}
void Entity::dropContainer(void) {}
void Entity::initBodyControl(void) {}
void Entity::jumpFromGround(class BlockSource const& bs) {}
void Entity::jumpFromGround(class PlayerMovementProxy& pmp, class BlockSource const& bs) {}
void Entity::newServerAiStep(void) {}
void Entity::_serverAiEntityStep(void) {}
void Entity::dropBags(void) {}
void Entity::tickDeath(void) {}
void Entity::updateGliding(void) {}
void Entity::_allowAscendingScaffolding(void) {}
void Entity::_getAdjustedAABBForSpawnCheck(AABB const& aabb, Vec3 const& v) {}
