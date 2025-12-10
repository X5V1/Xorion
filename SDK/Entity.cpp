#include "Entity.h"

#include "../Memory/GameData.h"
#include "../Utils/Utils.h"
#include "ClientInstance.h"
#include "BlockLegacy.h"
#include "Player.h"
#include "Tag.h"

// Helper macros for calling virtual functions through the game's vtable
// Entity is a memory overlay - we never construct it, only cast game memory to Entity*
// These macros properly call through the vtable with the correct calling convention

InventoryTransactionManager *Entity::getTransactionManager() {
	return reinterpret_cast<InventoryTransactionManager *>(reinterpret_cast<__int64>(this) + 0xEC8);
}

float Entity::getBlocksPerSecond() {
	return getTicksPerSecond() * *Game.getClientInstance()->minecraft->simTimer;
}

void Entity::lerpTo(Vec3 const &pos, Vec2 const &rot, int steps) {
	using lerpTo = void(__fastcall *)(Entity *, Vec3 const &, Vec2 const &, int);
	static lerpTo lerp = reinterpret_cast<lerpTo>(FindSignature("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 41 ? 48 8B F9 48 8B DA"));
	lerp(this, pos, rot, steps);
}

// Entity virtual method implementations - delegate to game vtable
// Note: Vtable indices are estimates and may need adjustment for Minecraft 1.21.124

int Entity::getStatusFlag(ActorFlags flag) { 
	using VFunc = int(__fastcall*)(void*, ActorFlags);
	return (*reinterpret_cast<VFunc**>(this))[1](this, flag);
}

void Entity::setStatusFlag(ActorFlags flag, bool val) { 
	using VFunc = void(__fastcall*)(void*, ActorFlags, bool);
	(*reinterpret_cast<VFunc**>(this))[2](this, flag, val);
}

int Entity::getLastHurtByEntity(void) {
	using VFunc = int(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[3](this);
}

void Entity::setLastHurtByEntity(Entity* e) {
	using VFunc = void(__fastcall*)(void*, Entity*);
	(*reinterpret_cast<VFunc**>(this))[4](this, e);
}

int Entity::getLastHurtByPlayer(void) {
	using VFunc = int(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[5](this);
}

void Entity::setLastHurtByPlayer(Player* p) {
	using VFunc = void(__fastcall*)(void*, Player*);
	(*reinterpret_cast<VFunc**>(this))[6](this, p);
}

int Entity::getLastHurtEntity(void) {
	using VFunc = int(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[7](this);
}

void Entity::setLastHurtEntity(Entity* e) {
	using VFunc = void(__fastcall*)(void*, Entity*);
	(*reinterpret_cast<VFunc**>(this))[8](this, e);
}

void Entity::outOfWorld(void) {
	using VFunc = void(__fastcall*)(void*);
	(*reinterpret_cast<VFunc**>(this))[9](this);
}

void Entity::reloadHardcoded(__int64 a, __int64 const& b) {
	using VFunc = void(__fastcall*)(void*, __int64, __int64 const&);
	(*reinterpret_cast<VFunc**>(this))[10](this, a, b);
}

void Entity::reloadHardcodedClient(__int64 a, __int64 const& b) {
	using VFunc = void(__fastcall*)(void*, __int64, __int64 const&);
	(*reinterpret_cast<VFunc**>(this))[11](this, a, b);
}

void Entity::initializeComponents(__int64 a, __int64 const& b) {
	using VFunc = void(__fastcall*)(void*, __int64, __int64 const&);
	(*reinterpret_cast<VFunc**>(this))[12](this, a, b);
}

void Entity::reloadComponents(__int64 a, __int64 const& b) {
	using VFunc = void(__fastcall*)(void*, __int64, __int64 const&);
	(*reinterpret_cast<VFunc**>(this))[13](this, a, b);
}

void Entity::_serverInitItemStackIds(void) {
	using VFunc = void(__fastcall*)(void*);
	(*reinterpret_cast<VFunc**>(this))[14](this);
}

void Entity::_doInitialMove(void) {
	using VFunc = void(__fastcall*)(void*);
	(*reinterpret_cast<VFunc**>(this))[15](this);
}

void Entity::hasComponent(HashedString const& h) {
	using VFunc = void(__fastcall*)(void*, HashedString const&);
	(*reinterpret_cast<VFunc**>(this))[16](this, h);
}

void Entity::Destructor() {
	using VFunc = void(__fastcall*)(void*);
	(*reinterpret_cast<VFunc**>(this))[17](this);
}

void Entity::resetUserPos(bool b) {
	using VFunc = void(__fastcall*)(void*, bool);
	(*reinterpret_cast<VFunc**>(this))[18](this, b);
}

int Entity::getOwnerEntityType(void) {
	using VFunc = int(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[19](this);
}

void Entity::remove(void) {
	using VFunc = void(__fastcall*)(void*);
	(*reinterpret_cast<VFunc**>(this))[20](this);
}

bool Entity::isRuntimePredictedMovementEnabled(void) {
	using VFunc = bool(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[21](this);
}

int Entity::getPredictedMovementValues(void) {
	using VFunc = int(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[22](this);
}

Vec3* Entity::getPos(void) {
	using VFunc = Vec3*(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[23](this);
}

Vec3* Entity::getPosOld(void) {
	using VFunc = Vec3*(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[24](this);
}

float Entity::getPosExtrapolated(float f) {
	using VFunc = float(__fastcall*)(void*, float);
	return (*reinterpret_cast<VFunc**>(this))[25](this, f);
}

float Entity::getAttachPos(__int64 a, float f) {
	using VFunc = float(__fastcall*)(void*, __int64, float);
	return (*reinterpret_cast<VFunc**>(this))[26](this, a, f);
}

int Entity::getFiringPos(void) {
	using VFunc = int(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[27](this);
}

void Entity::move(Vec3 const& v) {
	using VFunc = void(__fastcall*)(void*, Vec3 const&);
	(*reinterpret_cast<VFunc**>(this))[28](this, v);
}

void Entity::move(__int64& a, Vec3 const& v) {
	using VFunc = void(__fastcall*)(void*, __int64&, Vec3 const&);
	(*reinterpret_cast<VFunc**>(this))[29](this, a, v);
}

float Entity::getInterpolatedRidingPosition(float f) {
	using VFunc = float(__fastcall*)(void*, float);
	return (*reinterpret_cast<VFunc**>(this))[30](this, f);
}

float Entity::getInterpolatedBodyRot(float f) {
	using VFunc = float(__fastcall*)(void*, float);
	return (*reinterpret_cast<VFunc**>(this))[31](this, f);
}

float Entity::getInterpolatedHeadRot(float f) {
	using VFunc = float(__fastcall*)(void*, float);
	return (*reinterpret_cast<VFunc**>(this))[32](this, f);
}

float Entity::getInterpolatedBodyYaw(float f) {
	using VFunc = float(__fastcall*)(void*, float);
	return (*reinterpret_cast<VFunc**>(this))[33](this, f);
}

int Entity::getYawSpeedInDegreesPerSecond(void) {
	using VFunc = int(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[34](this);
}

float Entity::getInterpolatedWalkAnimSpeed(float f) {
	using VFunc = float(__fastcall*)(void*, float);
	return (*reinterpret_cast<VFunc**>(this))[35](this, f);
}

float Entity::getInterpolatedRidingOffset(float f, int i) {
	using VFunc = float(__fastcall*)(void*, float, int);
	return (*reinterpret_cast<VFunc**>(this))[36](this, f, i);
}

void Entity::resetInterpolated(void) {
	using VFunc = void(__fastcall*)(void*);
	(*reinterpret_cast<VFunc**>(this))[37](this);
}

void Entity::updateEntityInside(AABB const& a) {
	using VFunc = void(__fastcall*)(void*, AABB const&);
	(*reinterpret_cast<VFunc**>(this))[38](this, a);
}

void Entity::updateEntityInside(void) {
	using VFunc = void(__fastcall*)(void*);
	(*reinterpret_cast<VFunc**>(this))[39](this);
}

bool Entity::isFireImmune(void) {
	using VFunc = bool(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[40](this);
}

void Entity::breaksFallingBlocks(void) {
	using VFunc = void(__fastcall*)(void*);
	(*reinterpret_cast<VFunc**>(this))[41](this);
}

void Entity::blockedByShield(__int64 const& a, Entity& e) {
	using VFunc = void(__fastcall*)(void*, __int64 const&, Entity&);
	(*reinterpret_cast<VFunc**>(this))[42](this, a, e);
}

void Entity::canDisableShield(void) {
	using VFunc = void(__fastcall*)(void*);
	(*reinterpret_cast<VFunc**>(this))[43](this);
}

void Entity::teleportTo(Vec3 const& v, bool b1, int i1, int i2, bool b2) {
	using VFunc = void(__fastcall*)(void*, Vec3 const&, bool, int, int, bool);
	(*reinterpret_cast<VFunc**>(this))[44](this, v, b1, i1, i2, b2);
}

void Entity::tryTeleportTo(Vec3 const& v, bool b1, bool b2, int i1, int i2) {
	using VFunc = void(__fastcall*)(void*, Vec3 const&, bool, bool, int, int);
	(*reinterpret_cast<VFunc**>(this))[45](this, v, b1, b2, i1, i2);
}

void Entity::chorusFruitTeleport(Vec3 const& v) {
	using VFunc = void(__fastcall*)(void*, Vec3 const&);
	(*reinterpret_cast<VFunc**>(this))[46](this, v);
}

void Entity::lerpMotion(Vec3 const& v) {
	using VFunc = void(__fastcall*)(void*, Vec3 const&);
	(*reinterpret_cast<VFunc**>(this))[47](this, v);
}

void Entity::tryCreateAddEntityPacket(void) {
	using VFunc = void(__fastcall*)(void*);
	(*reinterpret_cast<VFunc**>(this))[48](this);
}

void Entity::normalTick(void) {
	using VFunc = void(__fastcall*)(void*);
	(*reinterpret_cast<VFunc**>(this))[49](this);
}

void Entity::baseTick(void) {
	using VFunc = void(__fastcall*)(void*);
	(*reinterpret_cast<VFunc**>(this))[50](this);
}

void Entity::passengerTick(void) {
	using VFunc = void(__fastcall*)(void*);
	(*reinterpret_cast<VFunc**>(this))[51](this);
}

void Entity::positionPassenger(Entity& e, float f) {
	using VFunc = void(__fastcall*)(void*, Entity&, float);
	(*reinterpret_cast<VFunc**>(this))[52](this, e, f);
}

void Entity::startRiding(Entity& e) {
	using VFunc = void(__fastcall*)(void*, Entity&);
	(*reinterpret_cast<VFunc**>(this))[53](this, e);
}

void Entity::addPassenger(Entity& e) {
	using VFunc = void(__fastcall*)(void*, Entity&);
	(*reinterpret_cast<VFunc**>(this))[54](this, e);
}

void Entity::flagPassengerToRemove(Entity& e) {
	using VFunc = void(__fastcall*)(void*, Entity&);
	(*reinterpret_cast<VFunc**>(this))[55](this, e);
}

int Entity::getExitTip(std::string const& s, __int64 a, __int64 b) {
	using VFunc = int(__fastcall*)(void*, std::string const&, __int64, __int64);
	return (*reinterpret_cast<VFunc**>(this))[56](this, s, a, b);
}

int Entity::getEntityLocNameString(void) {
	using VFunc = int(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[57](this);
}

void Entity::intersects(Vec3 const& v1, Vec3 const& v2) {
	using VFunc = void(__fastcall*)(void*, Vec3 const&, Vec3 const&);
	(*reinterpret_cast<VFunc**>(this))[58](this, v1, v2);
}

bool Entity::isInWall(void) {
	using VFunc = bool(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[59](this);
}

bool Entity::isInvisible(void) {
	using VFunc = bool(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[60](this);
}

bool Entity::canShowNameTag(void) {
	using VFunc = bool(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[61](this);
}

void Entity::canExistInPeaceful(void) {
	using VFunc = void(__fastcall*)(void*);
	(*reinterpret_cast<VFunc**>(this))[62](this);
}

void Entity::setNameTagVisible(bool b) {
	using VFunc = void(__fastcall*)(void*, bool);
	(*reinterpret_cast<VFunc**>(this))[63](this, b);
}

TextHolder* Entity::getNameTag(void) {
	using VFunc = TextHolder*(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[64](this);
}

int Entity::getNameTagAsHash(void) {
	using VFunc = int(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[65](this);
}

TextHolder* Entity::getFormattedNameTag(void) {
	using VFunc = TextHolder*(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[66](this);
}

void Entity::filterFormattedNameTag(__int64 const& a) {
	using VFunc = void(__fastcall*)(void*, __int64 const&);
	(*reinterpret_cast<VFunc**>(this))[67](this, a);
}

void Entity::setNameTag(std::string const& s) {
	using VFunc = void(__fastcall*)(void*, std::string const&);
	(*reinterpret_cast<VFunc**>(this))[68](this, s);
}

int Entity::getAlwaysShowNameTag(void) {
	using VFunc = int(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[69](this);
}

void Entity::setScoreTag(std::string const& s) {
	using VFunc = void(__fastcall*)(void*, std::string const&);
	(*reinterpret_cast<VFunc**>(this))[70](this, s);
}

int Entity::getScoreTag(void) {
	using VFunc = int(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[71](this);
}

bool Entity::isInWater(void) {
	using VFunc = bool(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[72](this);
}

bool Entity::isUnderLiquid(__int64 a) {
	using VFunc = bool(__fastcall*)(void*, __int64);
	return (*reinterpret_cast<VFunc**>(this))[73](this, a);
}

bool Entity::isOverWater(void) {
	using VFunc = bool(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[74](this);
}

void Entity::setBlockMovementSlowdownMultiplier(BlockLegacy const& b, Vec3 const& v) {
	using VFunc = void(__fastcall*)(void*, BlockLegacy const&, Vec3 const&);
	(*reinterpret_cast<VFunc**>(this))[75](this, b, v);
}

void Entity::resetBlockMovementSlowdownMultiplier(void) {
	using VFunc = void(__fastcall*)(void*);
	(*reinterpret_cast<VFunc**>(this))[76](this);
}

int Entity::getCameraOffset(void) {
	using VFunc = int(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[77](this);
}

int Entity::getShadowHeightOffs(void) {
	using VFunc = int(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[78](this);
}

int Entity::getShadowRadius(void) {
	using VFunc = int(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[79](this);
}

float Entity::getHeadLookVector(float f) {
	using VFunc = float(__fastcall*)(void*, float);
	return (*reinterpret_cast<VFunc**>(this))[80](this, f);
}

void Entity::canSeeInvisible(void) {
	using VFunc = void(__fastcall*)(void*);
	(*reinterpret_cast<VFunc**>(this))[81](this);
}

bool Entity::canSee(Vec3 const& v) {
	using VFunc = bool(__fastcall*)(void*, Vec3 const&);
	return (*reinterpret_cast<VFunc**>(this))[82](this, v);
}

bool Entity::canSee(Entity const& e) {
	using VFunc = bool(__fastcall*)(void*, Entity const&);
	return (*reinterpret_cast<VFunc**>(this))[83](this, e);
}

void Entity::canInteractWithOtherEntitiesInGame(void) {
	using VFunc = void(__fastcall*)(void*);
	(*reinterpret_cast<VFunc**>(this))[84](this);
}

bool Entity::isSkyLit(float f) {
	using VFunc = bool(__fastcall*)(void*, float);
	return (*reinterpret_cast<VFunc**>(this))[85](this, f);
}

float Entity::getBrightness(float f, __int64 const& a) {
	using VFunc = float(__fastcall*)(void*, float, __int64 const&);
	return (*reinterpret_cast<VFunc**>(this))[86](this, f, a);
}

void Entity::interactPreventDefault(void) {
	using VFunc = void(__fastcall*)(void*);
	(*reinterpret_cast<VFunc**>(this))[87](this);
}

void Entity::playerTouch(Player& p) {
	using VFunc = void(__fastcall*)(void*, Player&);
	(*reinterpret_cast<VFunc**>(this))[88](this, p);
}

void Entity::onAboveBubbleColumn(bool b) {
	using VFunc = void(__fastcall*)(void*, bool);
	(*reinterpret_cast<VFunc**>(this))[89](this, b);
}

void Entity::onInsideBubbleColumn(bool b) {
	using VFunc = void(__fastcall*)(void*, bool);
	(*reinterpret_cast<VFunc**>(this))[90](this, b);
}

bool Entity::isImmobile(void) {
	using VFunc = bool(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[91](this);
}

bool Entity::isSilent(void) {
	using VFunc = bool(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[92](this);
}

bool Entity::isSilentObserver(void) {
	using VFunc = bool(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[93](this);
}

bool Entity::isPickable(void) {
	using VFunc = bool(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[94](this);
}

bool Entity::isFishable(void) {
	using VFunc = bool(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[95](this);
}

bool Entity::isSleeping(void) {
	using VFunc = bool(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[96](this);
}

void Entity::setSleeping(bool b) {
	using VFunc = void(__fastcall*)(void*, bool);
	(*reinterpret_cast<VFunc**>(this))[97](this, b);
}

bool Entity::isShootable(void) {
	using VFunc = bool(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[98](this);
}

void Entity::setSneaking(bool b) {
	using VFunc = void(__fastcall*)(void*, bool);
	(*reinterpret_cast<VFunc**>(this))[99](this, b);
}

bool Entity::isBlocking(void) {
	using VFunc = bool(__fastcall*)(void*);
	return (*reinterpret_cast<VFunc**>(this))[100](this);
}

bool Entity::isDamageBlocked(__int64 const& a) {
	using VFunc = bool(__fastcall*)(void*, __int64 const&);
	return (*reinterpret_cast<VFunc**>(this))[101](this, a);
}
bool Entity::isAlive(void) { using VFunc = bool(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[102](this); }
bool Entity::isOnFire(void) { using VFunc = bool(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[103](this); }
bool Entity::isOnHotBlock(void) { using VFunc = bool(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[104](this); }
bool Entity::isCreativeModeAllowed(void) { using VFunc = bool(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[105](this); }
bool Entity::isSurfaceEntity(void) { using VFunc = bool(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[106](this); }
bool Entity::isTargetable(void) { using VFunc = bool(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[107](this); }
bool Entity::isLocalPlayer(void) { using VFunc = bool(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[108](this); }
bool Entity::isRemotePlayer(void) { using VFunc = bool(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[109](this); }
bool Entity::isPlayer(void) { using VFunc = bool(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[110](this); }
bool Entity::isAffectedByWaterBottle(void) { using VFunc = bool(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[111](this); }
bool Entity::canAttack(Entity* target, bool flag) { using VFunc = bool(__fastcall*)(void*, Entity*, bool); return (*reinterpret_cast<VFunc**>(this))[112](this, target, flag); }
void Entity::setTarget(Entity* target) { using VFunc = void(__fastcall*)(void*, Entity*); (*reinterpret_cast<VFunc**>(this))[113](this, target); }
bool Entity::isValidTarget(Entity* target) { using VFunc = bool(__fastcall*)(void*, Entity*); return (*reinterpret_cast<VFunc**>(this))[114](this, target); }
void Entity::attack(Entity& target, __int64 const& cause) { using VFunc = void(__fastcall*)(void*, Entity&, __int64 const&); (*reinterpret_cast<VFunc**>(this))[115](this, target, cause); }
void Entity::performRangedAttack(Entity& target, float power) { using VFunc = void(__fastcall*)(void*, Entity&, float); (*reinterpret_cast<VFunc**>(this))[116](this, target, power); }
int Entity::getEquipmentCount(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[117](this); }
void Entity::setOwner(uint64_t id) { using VFunc = void(__fastcall*)(void*, uint64_t); (*reinterpret_cast<VFunc**>(this))[118](this, id); }
void Entity::setSitting(bool sitting) { using VFunc = void(__fastcall*)(void*, bool); (*reinterpret_cast<VFunc**>(this))[119](this, sitting); }
void Entity::onTame(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[120](this); }
void Entity::onFailedTame(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[121](this); }
int Entity::getInventorySize(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[122](this); }
int Entity::getEquipSlots(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[123](this); }
int Entity::getChestSlots(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[124](this); }
void Entity::setStanding(bool standing) { using VFunc = void(__fastcall*)(void*, bool); (*reinterpret_cast<VFunc**>(this))[125](this, standing); }
void Entity::canPowerJump(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[126](this); }
void Entity::setCanPowerJump(bool canJump) { using VFunc = void(__fastcall*)(void*, bool); (*reinterpret_cast<VFunc**>(this))[127](this, canJump); }
bool Entity::isEnchanted(void) { using VFunc = bool(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[128](this); }
void Entity::vehicleLanded(Vec3 const& pos1, Vec3 const& pos2) { using VFunc = void(__fastcall*)(void*, Vec3 const&, Vec3 const&); (*reinterpret_cast<VFunc**>(this))[129](this, pos1, pos2); }
void Entity::shouldRender(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[130](this); }
void Entity::playAmbientSound(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[131](this); }
int Entity::getAmbientSound(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[132](this); }
bool Entity::isInvulnerableTo(__int64 const& cause) { using VFunc = bool(__fastcall*)(void*, __int64 const&); return (*reinterpret_cast<VFunc**>(this))[133](this, cause); }
int Entity::getBlockDamageCause(Block const& block) { using VFunc = int(__fastcall*)(void*, Block const&); return (*reinterpret_cast<VFunc**>(this))[134](this, block); }
void Entity::animateHurt(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[135](this); }
void Entity::doFireHurt(int amount) { using VFunc = void(__fastcall*)(void*, int); (*reinterpret_cast<VFunc**>(this))[136](this, amount); }
void Entity::onLightningHit(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[137](this); }
void Entity::onBounceStarted(Vec3i const& pos, Block const& block) { using VFunc = void(__fastcall*)(void*, Vec3i const&, Block const&); (*reinterpret_cast<VFunc**>(this))[138](this, pos, block); }
void Entity::feed(int amount) { using VFunc = void(__fastcall*)(void*, int); (*reinterpret_cast<VFunc**>(this))[139](this, amount); }
void Entity::handleEntityEvent(__int64 event, int data) { using VFunc = void(__fastcall*)(void*, __int64, int); (*reinterpret_cast<VFunc**>(this))[140](this, event, data); }
int Entity::getPickRadius(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[141](this); }
int Entity::getEntityRendererId(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[142](this); }
void Entity::spawnAtLocation(int id, int count) { using VFunc = void(__fastcall*)(void*, int, int); (*reinterpret_cast<VFunc**>(this))[143](this, id, count); }
void Entity::spawnAtLocation(int id, int count, float offset) { using VFunc = void(__fastcall*)(void*, int, int, float); (*reinterpret_cast<VFunc**>(this))[144](this, id, count, offset); }
void Entity::spawnAtLocation(ItemStack const& item, float offset) { using VFunc = void(__fastcall*)(void*, ItemStack const&, float); (*reinterpret_cast<VFunc**>(this))[145](this, item, offset); }
void Entity::despawn(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[146](this); }
void Entity::killed(Entity& killer) { using VFunc = void(__fastcall*)(void*, Entity&); (*reinterpret_cast<VFunc**>(this))[147](this, killer); }
void Entity::awardKillScore(Entity& victim, int score) { using VFunc = void(__fastcall*)(void*, Entity&, int); (*reinterpret_cast<VFunc**>(this))[148](this, victim, score); }
void Entity::setArmor(int slot, ItemStack const& item) { using VFunc = void(__fastcall*)(void*, int, ItemStack const&); (*reinterpret_cast<VFunc**>(this))[149](this, slot, item); }
ItemStack* Entity::getArmor(int slot) { using VFunc = ItemStack*(__fastcall*)(void*, int); return (*reinterpret_cast<VFunc**>(this))[150](this, slot); }
int Entity::getAllArmor(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[151](this); }
int Entity::getArmorMaterialTypeInSlot(int slot) { using VFunc = int(__fastcall*)(void*, int); return (*reinterpret_cast<VFunc**>(this))[152](this, slot); }
int Entity::getArmorMaterialTextureTypeInSlot(int slot) { using VFunc = int(__fastcall*)(void*, int); return (*reinterpret_cast<VFunc**>(this))[153](this, slot); }
int Entity::getArmorColorInSlot(int slot, int layer) { using VFunc = int(__fastcall*)(void*, int, int); return (*reinterpret_cast<VFunc**>(this))[154](this, slot, layer); }
int Entity::getEquippedSlot(int slot) { using VFunc = int(__fastcall*)(void*, int); return (*reinterpret_cast<VFunc**>(this))[155](this, slot); }
void Entity::setEquippedSlot(int slot, ItemStack const& item) { using VFunc = void(__fastcall*)(void*, int, ItemStack const&); (*reinterpret_cast<VFunc**>(this))[156](this, slot, item); }
void Entity::setCarriedItem(ItemStack const* item) { using VFunc = void(__fastcall*)(void*, ItemStack const*); (*reinterpret_cast<VFunc**>(this))[157](this, item); }
int Entity::getCarriedItem(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[158](this); }
void Entity::setOffhandSlot(ItemStack const* item) { using VFunc = void(__fastcall*)(void*, ItemStack const*); (*reinterpret_cast<VFunc**>(this))[159](this, item); }
ItemStack* Entity::getEquippedTotem(void) { using VFunc = ItemStack*(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[160](this); }
void Entity::consumeTotem(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[161](this); }
void Entity::save(CompoundTag* tag) { using VFunc = void(__fastcall*)(void*, CompoundTag*); (*reinterpret_cast<VFunc**>(this))[162](this, tag); }
void Entity::saveWithoutId(CompoundTag& tag) { using VFunc = void(__fastcall*)(void*, CompoundTag&); (*reinterpret_cast<VFunc**>(this))[163](this, tag); }
void Entity::load(CompoundTag const& tag, __int64& data) { using VFunc = void(__fastcall*)(void*, CompoundTag const&, __int64&); (*reinterpret_cast<VFunc**>(this))[164](this, tag, data); }
void Entity::loadLinks(CompoundTag const& tag, std::vector<__int64>& links, __int64& data) { using VFunc = void(__fastcall*)(void*, CompoundTag const&, std::vector<__int64>&, __int64&); (*reinterpret_cast<VFunc**>(this))[165](this, tag, links, data); }
int Entity::getEntityTypeId(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[166](this); }
void Entity::queryEntityRenderer(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[167](this); }
int Entity::getSourceUniqueID(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[168](this); }
void Entity::thawFreezeEffect(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[169](this); }
void Entity::canFreeze(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[170](this); }
bool Entity::isWearingLeatherArmor(void) { using VFunc = bool(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[171](this); }
int Entity::getLiquidAABB(__int64 aabb) { using VFunc = int(__fastcall*)(void*, __int64); return (*reinterpret_cast<VFunc**>(this))[172](this, aabb); }
void Entity::handleInsidePortal(Vec3i const& pos) { using VFunc = void(__fastcall*)(void*, Vec3i const&); (*reinterpret_cast<VFunc**>(this))[173](this, pos); }
int Entity::getPortalCooldown(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[174](this); }
int Entity::getPortalWaitTime(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[175](this); }
void Entity::canChangeDimensionsUsingPortal(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[176](this); }
void Entity::changeDimension(Dimension* dim, int changeType) { using VFunc = void(__fastcall*)(void*, Dimension*, int); (*reinterpret_cast<VFunc**>(this))[177](this, dim, changeType); }
void Entity::changeDimension(__int64 const& dimId) { using VFunc = void(__fastcall*)(void*, __int64 const&); (*reinterpret_cast<VFunc**>(this))[178](this, dimId); }
int Entity::getControllingPlayer(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[179](this); }
void Entity::checkFallDamage(float distance, bool flag) { using VFunc = void(__fastcall*)(void*, float, bool); (*reinterpret_cast<VFunc**>(this))[180](this, distance, flag); }
void Entity::causeFallDamage(float distance, float multiplier, __int64 cause) { using VFunc = void(__fastcall*)(void*, float, float, __int64); (*reinterpret_cast<VFunc**>(this))[181](this, distance, multiplier, cause); }
void Entity::handleFallDistanceOnServer(float distance, float multiplier, bool flag) { using VFunc = void(__fastcall*)(void*, float, float, bool); (*reinterpret_cast<VFunc**>(this))[182](this, distance, multiplier, flag); }
void Entity::playSynchronizedSound(__int64 sound, Vec3 const& pos, Block const& block, bool flag) { using VFunc = void(__fastcall*)(void*, __int64, Vec3 const&, Block const&, bool); (*reinterpret_cast<VFunc**>(this))[183](this, sound, pos, block, flag); }
void Entity::playSynchronizedSound(__int64 sound, Vec3 const& pos, int data, bool flag) { using VFunc = void(__fastcall*)(void*, __int64, Vec3 const&, int, bool); (*reinterpret_cast<VFunc**>(this))[184](this, sound, pos, data, flag); }
void Entity::onSynchedFlagUpdate(int flag, long oldVal, long newVal) { using VFunc = void(__fastcall*)(void*, int, long, long); (*reinterpret_cast<VFunc**>(this))[185](this, flag, oldVal, newVal); }
void Entity::onSynchedDataUpdate(int dataId) { using VFunc = void(__fastcall*)(void*, int); (*reinterpret_cast<VFunc**>(this))[186](this, dataId); }
void Entity::canAddPassenger(Entity& passenger) { using VFunc = void(__fastcall*)(void*, Entity&); (*reinterpret_cast<VFunc**>(this))[187](this, passenger); }
void Entity::canPickupItem(ItemStack const& item) { using VFunc = void(__fastcall*)(void*, ItemStack const&); (*reinterpret_cast<VFunc**>(this))[188](this, item); }
void Entity::canBePulledIntoVehicle(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[189](this); }
void Entity::inCaravan(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[190](this); }
bool Entity::isLeashableType(void) { using VFunc = bool(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[191](this); }
void Entity::tickLeash(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[192](this); }
void Entity::sendMotionPacketIfNeeded(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[193](this); }
void Entity::canSynchronizeNewEntity(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[194](this); }
void Entity::stopRiding(bool flag1, bool flag2, bool flag3) { using VFunc = void(__fastcall*)(void*, bool, bool, bool); (*reinterpret_cast<VFunc**>(this))[195](this, flag1, flag2, flag3); }
void Entity::startSwimming(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[196](this); }
void Entity::stopSwimming(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[197](this); }
void Entity::buildDebugInfo(std::string& info) { using VFunc = void(__fastcall*)(void*, std::string&); (*reinterpret_cast<VFunc**>(this))[198](this, info); }
int Entity::getCommandPermissionLevel(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[199](this); }
bool Entity::isClientSide(void) { using VFunc = bool(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[200](this); }
class AttributeInstance* Entity::getMutableAttribute(class Attribute* attr) { using VFunc = class AttributeInstance*(__fastcall*)(void*, class Attribute*); return (*reinterpret_cast<VFunc**>(this))[201](this, attr); }
class AttributeInstance* Entity::getAttribute(class Attribute* attr) { using VFunc = class AttributeInstance*(__fastcall*)(void*, class Attribute*); return (*reinterpret_cast<VFunc**>(this))[202](this, attr); }
int Entity::getDeathTime(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[203](this); }
void Entity::heal(int amount) { using VFunc = void(__fastcall*)(void*, int); (*reinterpret_cast<VFunc**>(this))[204](this, amount); }
bool Entity::isInvertedHealAndHarm(void) { using VFunc = bool(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[205](this); }
void Entity::canBeAffected(int effect) { using VFunc = void(__fastcall*)(void*, int); (*reinterpret_cast<VFunc**>(this))[206](this, effect); }
void Entity::canBeAffectedByArrow(MobEffectInstance const& effect) { using VFunc = void(__fastcall*)(void*, MobEffectInstance const&); (*reinterpret_cast<VFunc**>(this))[207](this, effect); }
void Entity::onEffectAdded(MobEffectInstance& effect) { using VFunc = void(__fastcall*)(void*, MobEffectInstance&); (*reinterpret_cast<VFunc**>(this))[208](this, effect); }
void Entity::onEffectUpdated(MobEffectInstance& effect) { using VFunc = void(__fastcall*)(void*, MobEffectInstance&); (*reinterpret_cast<VFunc**>(this))[209](this, effect); }
void Entity::onEffectRemoved(MobEffectInstance& effect) { using VFunc = void(__fastcall*)(void*, MobEffectInstance&); (*reinterpret_cast<VFunc**>(this))[210](this, effect); }
void Entity::canObstructSpawningAndBlockPlacement(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[211](this); }
class AnimationComponent* Entity::getAnimationComponent(void) { using VFunc = class AnimationComponent*(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[212](this); }
void Entity::openContainerComponent(Player& player) { using VFunc = void(__fastcall*)(void*, Player&); (*reinterpret_cast<VFunc**>(this))[213](this, player); }
void Entity::swing(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[214](this); }
void Entity::useItem(ItemStack& item, __int64 useMethod, bool flag) { using VFunc = void(__fastcall*)(void*, ItemStack&, __int64, bool); (*reinterpret_cast<VFunc**>(this))[215](this, item, useMethod, flag); }
void Entity::hasOutputSignal(char signal) { using VFunc = void(__fastcall*)(void*, char); (*reinterpret_cast<VFunc**>(this))[216](this, signal); }
int Entity::getOutputSignal(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[217](this); }
int Entity::getDebugText(std::vector<std::string>& text) { using VFunc = int(__fastcall*)(void*, std::vector<std::string>&); return (*reinterpret_cast<VFunc**>(this))[218](this, text); }
int Entity::getMapDecorationRotation(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[219](this); }
int Entity::getPassengerYRotation(Entity const& passenger) { using VFunc = int(__fastcall*)(void*, Entity const&); return (*reinterpret_cast<VFunc**>(this))[220](this, passenger); }
void Entity::add(ItemStack& item) { using VFunc = void(__fastcall*)(void*, ItemStack&); (*reinterpret_cast<VFunc**>(this))[221](this, item); }
void Entity::drop(ItemStack const& item, bool flag) { using VFunc = void(__fastcall*)(void*, ItemStack const&, bool); (*reinterpret_cast<VFunc**>(this))[222](this, item, flag); }
int Entity::getInteraction(Player& player, __int64& action, Vec3 const& pos) { using VFunc = int(__fastcall*)(void*, Player&, __int64&, Vec3 const&); return (*reinterpret_cast<VFunc**>(this))[223](this, player, action, pos); }
void Entity::canDestroyBlock(Block const& block) { using VFunc = void(__fastcall*)(void*, Block const&); (*reinterpret_cast<VFunc**>(this))[224](this, block); }
void Entity::setAuxValue(int value) { using VFunc = void(__fastcall*)(void*, int); (*reinterpret_cast<VFunc**>(this))[225](this, value); }
void Entity::setSize(float width, float height) { using VFunc = void(__fastcall*)(void*, float, float); (*reinterpret_cast<VFunc**>(this))[226](this, width, height); }
void Entity::onOrphan(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[227](this); }
void Entity::wobble(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[228](this); }
void Entity::wasHurt(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[229](this); }
void Entity::startSpinAttack(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[230](this); }
void Entity::stopSpinAttack(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[231](this); }
void Entity::setDamageNearbyEntitys(bool flag) { using VFunc = void(__fastcall*)(void*, bool); (*reinterpret_cast<VFunc**>(this))[232](this, flag); }
void Entity::renderDebugServerState(__int64 const& state) { using VFunc = void(__fastcall*)(void*, __int64 const&); (*reinterpret_cast<VFunc**>(this))[233](this, state); }
void Entity::reloadLootTable(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[234](this); }
void Entity::reloadLootTable(__int64 const& table) { using VFunc = void(__fastcall*)(void*, __int64 const&); (*reinterpret_cast<VFunc**>(this))[235](this, table); }
int Entity::getDeletionDelayTimeSeconds(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[236](this); }
void Entity::kill(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[237](this); }
void Entity::die(__int64 const& cause) { using VFunc = void(__fastcall*)(void*, __int64 const&); (*reinterpret_cast<VFunc**>(this))[238](this, cause); }
void Entity::shouldDropDeathLoot(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[239](this); }
void Entity::shouldTick(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[240](this); }
void Entity::applySnapshot(__int64 const& snapshot1, __int64 const& snapshot2) { using VFunc = void(__fastcall*)(void*, __int64 const&, __int64 const&); (*reinterpret_cast<VFunc**>(this))[241](this, snapshot1, snapshot2); }
float Entity::getNextStep(float step) { using VFunc = float(__fastcall*)(void*, float); return (*reinterpret_cast<VFunc**>(this))[242](this, step); }
int Entity::getLootTable(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[243](this); }
void Entity::onPush(Entity& other) { using VFunc = void(__fastcall*)(void*, Entity&); (*reinterpret_cast<VFunc**>(this))[244](this, other); }
Vec3i Entity::getLastDeathPos(void) { using VFunc = Vec3i(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[245](this); }
int Entity::getLastDeathDimension(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[246](this); }
void Entity::hasDiedBefore(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[247](this); }
void Entity::doWaterSplashEffect(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[248](this); }
void Entity::_shouldProvideFeedbackOnHandContainerItemSet(int slot, ItemStack const& item) { using VFunc = void(__fastcall*)(void*, int, ItemStack const&); (*reinterpret_cast<VFunc**>(this))[249](this, slot, item); }
void Entity::_shouldProvideFeedbackOnArmorSet(int slot, ItemStack const& item) { using VFunc = void(__fastcall*)(void*, int, ItemStack const&); (*reinterpret_cast<VFunc**>(this))[250](this, slot, item); }
void Entity::updateEntitySpecificMolangVariables(__int64& vars) { using VFunc = void(__fastcall*)(void*, __int64&); (*reinterpret_cast<VFunc**>(this))[251](this, vars); }
void Entity::shouldTryMakeStepSound(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[252](this); }
void Entity::canMakeStepSound(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[253](this); }
void Entity::_hurt(__int64 const& cause, float damage, bool flag1, bool flag2) { using VFunc = void(__fastcall*)(void*, __int64 const&, float, bool, bool); (*reinterpret_cast<VFunc**>(this))[254](this, cause, damage, flag1, flag2); }
void Entity::markHurt(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[255](this); }
void Entity::_getAnimationComponent(__int64& component, __int64 data) { using VFunc = void(__fastcall*)(void*, __int64&, __int64); (*reinterpret_cast<VFunc**>(this))[256](this, component, data); }
void Entity::readAdditionalSaveData(CompoundTag const& tag, __int64& data) { using VFunc = void(__fastcall*)(void*, CompoundTag const&, __int64&); (*reinterpret_cast<VFunc**>(this))[257](this, tag, data); }
void Entity::addAdditionalSaveData(CompoundTag& tag) { using VFunc = void(__fastcall*)(void*, CompoundTag&); (*reinterpret_cast<VFunc**>(this))[258](this, tag); }
void Entity::_playStepSound(Vec3i const& pos, Block const& block) { using VFunc = void(__fastcall*)(void*, Vec3i const&, Block const&); (*reinterpret_cast<VFunc**>(this))[259](this, pos, block); }
void Entity::_playFlySound(Vec3i const& pos, Block const& block) { using VFunc = void(__fastcall*)(void*, Vec3i const&, Block const&); (*reinterpret_cast<VFunc**>(this))[260](this, pos, block); }
void Entity::_makeFlySound(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[261](this); }
void Entity::checkInsideBlocks(float delta) { using VFunc = void(__fastcall*)(void*, float); (*reinterpret_cast<VFunc**>(this))[262](this, delta); }
void Entity::pushOutOfBlocks(Vec3 const& pos) { using VFunc = void(__fastcall*)(void*, Vec3 const&); (*reinterpret_cast<VFunc**>(this))[263](this, pos); }
void Entity::spawnTrailBubbles(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[264](this); }
void Entity::updateInsideBlock(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[265](this); }
void Entity::_removePassenger(uint64_t const& id, bool flag1, bool flag2, bool flag3) { using VFunc = void(__fastcall*)(void*, uint64_t const&, bool, bool, bool); (*reinterpret_cast<VFunc**>(this))[266](this, id, flag1, flag2, flag3); }
void Entity::_onSizeUpdated(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[267](this); }
void Entity::_doAutoAttackOnTouch(Entity& target) { using VFunc = void(__fastcall*)(void*, Entity&); (*reinterpret_cast<VFunc**>(this))[268](this, target); }
void Entity::knockback(Entity* attacker, int damage, float x, float y, float z, float w, float power) { using VFunc = void(__fastcall*)(void*, Entity*, int, float, float, float, float, float); (*reinterpret_cast<VFunc**>(this))[269](this, attacker, damage, x, y, z, w, power); }
void Entity::spawnAnim(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[270](this); }
void Entity::setSprinting(bool sprinting) { using VFunc = void(__fastcall*)(void*, bool); (*reinterpret_cast<VFunc**>(this))[271](this, sprinting); }
int Entity::getHurtSound(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[272](this); }
int Entity::getDeathSound(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[273](this); }
__int64 Entity::getSpeed(void) { using VFunc = __int64(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[274](this); }
void Entity::setSpeed(float speed) { using VFunc = void(__fastcall*)(void*, float); (*reinterpret_cast<VFunc**>(this))[275](this, speed); }
void Entity::hurtEffects(__int64 const& cause, float damage, bool flag1, bool flag2) { using VFunc = void(__fastcall*)(void*, __int64 const&, float, bool, bool); (*reinterpret_cast<VFunc**>(this))[276](this, cause, damage, flag1, flag2); }
int Entity::getMeleeWeaponDamageBonus(Entity target) { using VFunc = int(__fastcall*)(void*, Entity); return (*reinterpret_cast<VFunc**>(this))[277](this, target); }
int Entity::getMeleeKnockbackBonus(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[278](this); }
void Entity::travel(float x, float y, float z, bool flag) { using VFunc = void(__fastcall*)(void*, float, float, float, bool); (*reinterpret_cast<VFunc**>(this))[279](this, x, y, z, flag); }
void Entity::applyFinalFriction(float friction, bool flag) { using VFunc = void(__fastcall*)(void*, float, bool); (*reinterpret_cast<VFunc**>(this))[280](this, friction, flag); }
void Entity::aiStep(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[281](this); }
void Entity::aiStep(__int64& data) { using VFunc = void(__fastcall*)(void*, __int64&); (*reinterpret_cast<VFunc**>(this))[282](this, data); }
void Entity::pushEntitys(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[283](this); }
void Entity::lookAt(Entity* target, float yaw, float pitch) { using VFunc = void(__fastcall*)(void*, Entity*, float, float); (*reinterpret_cast<VFunc**>(this))[284](this, target, yaw, pitch); }
bool Entity::isLookingAtAnEntity(void) { using VFunc = bool(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[285](this); }
void Entity::checkSpawnRules(bool flag) { using VFunc = void(__fastcall*)(void*, bool); (*reinterpret_cast<VFunc**>(this))[286](this, flag); }
void Entity::checkSpawnObstruction(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[287](this); }
float Entity::getAttackAnim(float delta) { using VFunc = float(__fastcall*)(void*, float); return (*reinterpret_cast<VFunc**>(this))[288](this, delta); }
int Entity::getItemUseDuration(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[289](this); }
int Entity::getItemUseStartupProgress(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[290](this); }
int Entity::getItemUseIntervalProgress(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[291](this); }
int Entity::getItemUseIntervalAxis(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[292](this); }
int Entity::getTimeAlongSwing(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[293](this); }
void Entity::ate(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[294](this); }
int Entity::getMaxHeadXRot(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[295](this); }
bool Entity::isAlliedTo(Entity* entity) { using VFunc = bool(__fastcall*)(void*, Entity*); return (*reinterpret_cast<VFunc**>(this))[296](this, entity); }
void Entity::doHurtTarget(Entity* target, __int64 const& cause) { using VFunc = void(__fastcall*)(void*, Entity*, __int64 const&); (*reinterpret_cast<VFunc**>(this))[297](this, target, cause); }
void Entity::canBeControlledByPassenger(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[298](this); }
void Entity::leaveCaravan(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[299](this); }
void Entity::joinCaravan(Entity entity) { using VFunc = void(__fastcall*)(void*, Entity); (*reinterpret_cast<VFunc**>(this))[300](this, entity); }
void Entity::hasCaravanTail(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[301](this); }
int Entity::getCaravanHead(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[302](this); }
int Entity::getArmorValue(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[303](this); }
int Entity::getArmorCoverPercentage(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[304](this); }
int Entity::getToughnessValue(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[305](this); }
void Entity::hurtArmorSlots(__int64 const& cause, int amount, std::bitset<4> slots) { using VFunc = void(__fastcall*)(void*, __int64 const&, int, std::bitset<4>); (*reinterpret_cast<VFunc**>(this))[306](this, cause, amount, slots); }
void Entity::setDamagedArmor(int slot, ItemStack const& item) { using VFunc = void(__fastcall*)(void*, int, ItemStack const&); (*reinterpret_cast<VFunc**>(this))[307](this, slot, item); }
void Entity::sendArmorDamage(std::bitset<4> slots) { using VFunc = void(__fastcall*)(void*, std::bitset<4>); (*reinterpret_cast<VFunc**>(this))[308](this, slots); }
void Entity::sendArmor(std::bitset<4> slots) { using VFunc = void(__fastcall*)(void*, std::bitset<4>); (*reinterpret_cast<VFunc**>(this))[309](this, slots); }
void Entity::containerChanged(int slot) { using VFunc = void(__fastcall*)(void*, int); (*reinterpret_cast<VFunc**>(this))[310](this, slot); }
void Entity::updateEquipment(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[311](this); }
void Entity::clearEquipment(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[312](this); }
int Entity::getAllArmorID(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[313](this); }
int Entity::getAllHand(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[314](this); }
int Entity::getAllEquipment(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[315](this); }
int Entity::getArmorTypeHash(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[316](this); }
void Entity::dropEquipmentOnDeath(__int64 const& cause, int amount) { using VFunc = void(__fastcall*)(void*, __int64 const&, int); (*reinterpret_cast<VFunc**>(this))[317](this, cause, amount); }
void Entity::dropEquipmentOnDeath(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[318](this); }
void Entity::clearVanishEnchantedItemsOnDeath(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[319](this); }
void Entity::sendInventory(bool shouldSelectSlot) { using VFunc = void(__fastcall*)(void*, bool); (*reinterpret_cast<VFunc**>(this))[320](this, shouldSelectSlot); }
float Entity::getDamageAfterEnchantReduction(__int64 const& cause, float damage) { using VFunc = float(__fastcall*)(void*, __int64 const&, float); return (*reinterpret_cast<VFunc**>(this))[321](this, cause, damage); }
float Entity::getDamageAfterArmorReduction(__int64 const& cause, float damage) { using VFunc = float(__fastcall*)(void*, __int64 const&, float); return (*reinterpret_cast<VFunc**>(this))[322](this, cause, damage); }
float Entity::getDamageAfterResistanceEffect(__int64 const& cause, float damage) { using VFunc = float(__fastcall*)(void*, __int64 const&, float); return (*reinterpret_cast<VFunc**>(this))[323](this, cause, damage); }
void Entity::createAIGoals(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[324](this); }
void Entity::onBorn(Entity& parent1, Entity& parent2) { using VFunc = void(__fastcall*)(void*, Entity&, Entity&); (*reinterpret_cast<VFunc**>(this))[325](this, parent1, parent2); }
void Entity::setItemSlot(int slot, ItemStack const& item) { using VFunc = void(__fastcall*)(void*, int, ItemStack const&); (*reinterpret_cast<VFunc**>(this))[326](this, slot, item); }
void Entity::setTransitioningSitting(bool sitting) { using VFunc = void(__fastcall*)(void*, bool); (*reinterpret_cast<VFunc**>(this))[327](this, sitting); }
void Entity::attackAnimation(Entity* target, float power) { using VFunc = void(__fastcall*)(void*, Entity*, float); (*reinterpret_cast<VFunc**>(this))[328](this, target, power); }
int Entity::getAttackTime(void) { using VFunc = int(__fastcall*)(void*); return (*reinterpret_cast<VFunc**>(this))[329](this); }
void Entity::_getWalkTargetValue(Vec3i const& pos) { using VFunc = void(__fastcall*)(void*, Vec3i const&); (*reinterpret_cast<VFunc**>(this))[330](this, pos); }
void Entity::canExistWhenDisallowEntity(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[331](this); }
void Entity::ascendLadder(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[332](this); }
void Entity::ascendBlockByJumping(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[333](this); }
void Entity::descendBlockByCrouching(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[334](this); }
void Entity::dropContainer(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[335](this); }
void Entity::initBodyControl(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[336](this); }
void Entity::jumpFromGround(BlockSource const& source) { using VFunc = void(__fastcall*)(void*, BlockSource const&); (*reinterpret_cast<VFunc**>(this))[337](this, source); }
void Entity::jumpFromGround(PlayerMovementProxy& proxy, BlockSource const& source) { using VFunc = void(__fastcall*)(void*, PlayerMovementProxy&, BlockSource const&); (*reinterpret_cast<VFunc**>(this))[338](this, proxy, source); }
void Entity::newServerAiStep(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[339](this); }
void Entity::_serverAiEntityStep(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[340](this); }
void Entity::dropBags(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[341](this); }
void Entity::tickDeath(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[342](this); }
void Entity::updateGliding(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[343](this); }
void Entity::_allowAscendingScaffolding(void) { using VFunc = void(__fastcall*)(void*); (*reinterpret_cast<VFunc**>(this))[344](this); }
void Entity::_getAdjustedAABBForSpawnCheck(AABB const& aabb, Vec3 const& pos) { using VFunc = void(__fastcall*)(void*, AABB const&, Vec3 const&); (*reinterpret_cast<VFunc**>(this))[345](this, aabb, pos); }
