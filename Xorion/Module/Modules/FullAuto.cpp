#include "FullAuto.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/GameMode.h"
#include "../../../SDK/Attribute.h"
#include "../../../Utils/Target.h"
#include <random>
#include <algorithm>
#include <mutex>
#include <limits>

// Forward declarations for module types
class FullBright;
class AutoSprint;
class Aimbot;
class AutoClicker;
class AirJump;
class Jetpack;
class Velocity;
class Fucker;
class Scaffold;

// Constants
static constexpr int AUTOCLICKER_BURST_DURATION_TICKS = 50;  // 2.5 seconds at 20 ticks/sec
static constexpr float HEALTH_THRESHOLD = 8.0f;  // Health threshold for NoKnockback
static constexpr float VOID_Y_THRESHOLD = 10.0f;  // Y position threshold for void detection
static constexpr float FALL_VELOCITY_THRESHOLD = -0.5f;  // Falling velocity threshold
static constexpr float FALL_DISTANCE_MULTIPLIER = 3.0f;  // Multiplier for estimating fall distance

// Static random generator for better performance (with thread safety)
static std::random_device rd;
static std::mt19937 gen(rd());
static std::mutex rng_mutex;  // Protect random number generation

FullAuto::FullAuto() : IModule(0, Category::COMBAT, "Full automation module with AI-controlled gameplay") {
	// Register settings
	registerBoolSetting("Perfection Mode", &this->perfectionMode, this->perfectionMode);
	registerIntSetting("Hacks", &this->hacksMode, this->hacksMode, 0, 2);
	registerIntSetting("Mode", &this->mode, this->mode, 0, 1);
}

FullAuto::~FullAuto() {}

const char* FullAuto::getModuleName() {
	return "FullAuto";
}

void FullAuto::onEnable() {
	// Reset state when enabled
	autoclickerBurstTicks = 0;
	isInBurst = false;
	
	// Safety check for moduleMgr
	if (!moduleMgr) return;
	
	// Enable base modules based on hack mode
	if (hacksMode == 0) {
		disableAllHacks();
		// Enable only fullbright and autosprint
		if (auto fb = moduleMgr->getModule<FullBright>()) fb->setEnabled(true);
		if (auto as = moduleMgr->getModule<AutoSprint>()) as->setEnabled(true);
	} else if (hacksMode == 1) {
		enableLowRiskHacks();
	} else if (hacksMode == 2) {
		enableFullHacks();
	}
}

void FullAuto::onDisable() {
	// Safety check for moduleMgr
	if (!moduleMgr) return;
	
	// Disable all automation-controlled modules
	disableAllHacks();
}

void FullAuto::onTick(C_GameMode* gm) {
	if (!gm || !gm->player || !moduleMgr) return;
	
	Entity* player = gm->player;
	
	// Update hack modes dynamically
	if (hacksMode == 0) {
		// Off: only Fullbright and AutoSprint
		if (auto fb = moduleMgr->getModule<FullBright>()) 
			if (!fb->isEnabled()) fb->setEnabled(true);
		if (auto as = moduleMgr->getModule<AutoSprint>()) 
			if (!as->isEnabled()) as->setEnabled(true);
	} else if (hacksMode == 1) {
		// Low risk: manage burst autoclicker
		if (isInBurst) {
			autoclickerBurstTicks++;
			if (autoclickerBurstTicks >= AUTOCLICKER_BURST_DURATION_TICKS) {
				isInBurst = false;
				if (auto ac = moduleMgr->getModule<AutoClicker>()) ac->setEnabled(false);
			}
		}
	}
	
	// Execute mode behavior
	if (mode == 0) {
		handleKillMode(player, gm);
	} else if (mode == 1) {
		handleRunMode(player, gm);
	}
}

void FullAuto::onPlayerTick(Player* player) {
	if (!player || !moduleMgr) return;
	
	// Handle air jump for safety in low risk and full modes
	if (hacksMode >= 1) {
		if (shouldAirJumpForSafety(player)) {
			if (auto aj = moduleMgr->getModule<AirJump>()) {
				if (!aj->isEnabled()) aj->setEnabled(true);
			}
		}
	}
}

void FullAuto::handleKillMode(Entity* player, C_GameMode* gm) {
	Entity* target = findNearestPlayer(player);
	
	if (!target) return;
	
	// Enable combat modules based on hack level
	if (hacksMode == 2) {  // Full hacks
		// Enable aimbot
		if (auto ab = moduleMgr->getModule<Aimbot>()) {
			if (!ab->isEnabled()) ab->setEnabled(true);
		}
		
		// Enable autoclicker
		if (auto ac = moduleMgr->getModule<AutoClicker>()) {
			if (!ac->isEnabled()) ac->setEnabled(true);
		}
		
		// Use jetpack to reach target if needed
		Vec3 targetPos = *target->getPos();
		Vec3 playerPos = *player->getPos();
		float distance = playerPos.dist(targetPos);
		
		if (distance > 10.0f) {
			if (auto jp = moduleMgr->getModule<Jetpack>()) {
				if (!jp->isEnabled()) jp->setEnabled(true);
			}
		}
		
	} else if (hacksMode == 1) {  // Low risk
		// Only enable aimbot if opponent is sidestepping
		if (isOpponentSidestepping(target)) {
			if (auto ab = moduleMgr->getModule<Aimbot>()) {
				if (!ab->isEnabled()) ab->setEnabled(true);
			}
		}
		
		// Start autoclicker burst if not already in one
		if (!isInBurst) {
			isInBurst = true;
			autoclickerBurstTicks = 0;
			if (auto ac = moduleMgr->getModule<AutoClicker>()) {
				ac->setEnabled(true);
			}
		}
		
		// Enable NoKnockback temporarily if health > HEALTH_THRESHOLD
		static HealthAttribute healthAttr;
		auto healthInst = player->getAttribute(&healthAttr);
		if (healthInst && healthInst->currentValue > HEALTH_THRESHOLD) {
			if (auto vel = moduleMgr->getModule<Velocity>()) {
				if (!vel->isEnabled()) vel->setEnabled(true);
			}
		}
	}
	
	// Enable Fucker with probability control if not in perfection mode
	if (hacksMode >= 1) {
		if (auto fucker = moduleMgr->getModule<Fucker>()) {
			if (perfectionMode) {
				// In perfection mode, fucker works perfectly
				if (!fucker->isEnabled()) fucker->setEnabled(true);
			} else if (hacksMode == 1) {
				// Low risk: use fucker 2/10 times (thread-safe)
				static int fuckerCounter = 0;
				static std::mutex fuckerMutex;
				{
					std::lock_guard<std::mutex> lock(fuckerMutex);
					bool shouldEnable = ((fuckerCounter++ % 10) < 2);
					if (shouldEnable && !fucker->isEnabled()) {
						fucker->setEnabled(true);
					} else if (!shouldEnable && fucker->isEnabled()) {
						fucker->setEnabled(false);
					}
				}
			} else {
				// Full hacks: fucker works flawlessly
				if (!fucker->isEnabled()) fucker->setEnabled(true);
			}
		}
	}
}

void FullAuto::handleRunMode(Entity* player, C_GameMode* gm) {
	findAndRunFromPlayers(player, gm);
}

void FullAuto::enableLowRiskHacks() {
	if (!moduleMgr) return;
	
	// Enable base modules
	if (auto fb = moduleMgr->getModule<FullBright>()) fb->setEnabled(true);
	if (auto as = moduleMgr->getModule<AutoSprint>()) as->setEnabled(true);
	
	// Other low-risk hacks are enabled conditionally in onTick
}

void FullAuto::enableFullHacks() {
	if (!moduleMgr) return;
	
	// Enable base modules
	if (auto fb = moduleMgr->getModule<FullBright>()) fb->setEnabled(true);
	if (auto as = moduleMgr->getModule<AutoSprint>()) as->setEnabled(true);
	
	// Enable scaffold for bridging
	if (auto sc = moduleMgr->getModule<Scaffold>()) sc->setEnabled(true);
	
	// Other modules are enabled on-demand in handleKillMode
}

void FullAuto::disableAllHacks() {
	if (!moduleMgr) return;
	
	// Disable all automation-managed modules
	if (auto ab = moduleMgr->getModule<Aimbot>()) ab->setEnabled(false);
	if (auto ac = moduleMgr->getModule<AutoClicker>()) ac->setEnabled(false);
	if (auto aj = moduleMgr->getModule<AirJump>()) aj->setEnabled(false);
	if (auto fb = moduleMgr->getModule<FullBright>()) fb->setEnabled(false);
	if (auto as = moduleMgr->getModule<AutoSprint>()) as->setEnabled(false);
	if (auto jp = moduleMgr->getModule<Jetpack>()) jp->setEnabled(false);
	if (auto vel = moduleMgr->getModule<Velocity>()) vel->setEnabled(false);
	if (auto sc = moduleMgr->getModule<Scaffold>()) sc->setEnabled(false);
	if (auto fucker = moduleMgr->getModule<Fucker>()) fucker->setEnabled(false);
}

bool FullAuto::shouldPlaceFail() {
	if (perfectionMode) return false;
	static std::uniform_int_distribution<> dis(1, 18);
	std::lock_guard<std::mutex> lock(rng_mutex);
	return dis(gen) == 1;  // 1/18 chance
}

bool FullAuto::shouldAimbotMiss() {
	if (perfectionMode) return false;
	static std::uniform_int_distribution<> dis(1, 6);
	std::lock_guard<std::mutex> lock(rng_mutex);
	return dis(gen) == 1;  // 1/6 chance
}

bool FullAuto::shouldFuckerIgnore() {
	if (perfectionMode) return false;
	static std::uniform_int_distribution<> dis(1, 100);
	std::lock_guard<std::mutex> lock(rng_mutex);
	return dis(gen) > 70;  // 30% chance to ignore
}

Entity* FullAuto::findNearestPlayer(Entity* localPlayer) {
	if (!localPlayer) return nullptr;
	
	Entity* nearestPlayer = nullptr;
	float nearestDistance = std::numeric_limits<float>::max();
	Vec3 localPos = *localPlayer->getPos();
	
	g_Data.forEachPlayer([&](Entity* ent, bool) {
		if (ent == localPlayer) return;
		if (!Target::isValidTarget(ent)) return;
		
		Vec3 entPos = *ent->getPos();
		float distance = localPos.dist(entPos);
		
		if (distance < nearestDistance) {
			nearestDistance = distance;
			nearestPlayer = ent;
		}
	});
	
	return nearestPlayer;
}

void FullAuto::findAndRunFromPlayers(Entity* localPlayer, C_GameMode* gm) {
	if (!localPlayer) return;
	
	Vec3 localPos = *localPlayer->getPos();
	std::vector<std::pair<Entity*, float>> playersWithArmor;
	
	// Find top 3 players with best armor within 50 blocks
	g_Data.forEachPlayer([&](Entity* ent, bool) {
		if (ent == localPlayer) return;
		if (!Target::isValidTarget(ent)) return;
		
		Vec3 entPos = *ent->getPos();
		float distance = localPos.dist(entPos);
		
		if (distance <= 50.0f) {
			// Calculate armor score using getArmorValue
			float armorScore = (float)ent->getArmorValue();
			playersWithArmor.push_back({ent, armorScore});
		}
	});
	
	// Sort by armor score and get top 3
	std::sort(playersWithArmor.begin(), playersWithArmor.end(),
		[](const std::pair<Entity*, float>& a, const std::pair<Entity*, float>& b) {
			return a.second > b.second;
		});
	
	if (playersWithArmor.empty()) return;
	
	// Calculate average position of top 3 threats
	Vec3 threatCenter(0, 0, 0);
	int count = std::min(3, (int)playersWithArmor.size());
	for (int i = 0; i < count; i++) {
		Vec3 pos = *playersWithArmor[i].first->getPos();
		threatCenter = threatCenter.add(pos);
	}
	threatCenter = threatCenter.div((float)count);
	
	// Run in opposite direction
	Vec3 runDirection = localPos.sub(threatCenter);
	runDirection = runDirection.normalize();
	
	// Enable scaffold for bridging if needed
	if (hacksMode >= 1) {
		if (auto sc = moduleMgr->getModule<Scaffold>()) {
			if (!sc->isEnabled()) sc->setEnabled(true);
		}
	}
	
	// Use jetpack if in full hacks mode
	if (hacksMode == 2) {
		if (auto jp = moduleMgr->getModule<Jetpack>()) {
			if (!jp->isEnabled()) jp->setEnabled(true);
		}
	}
}

bool FullAuto::isOpponentSidestepping(Entity* target) {
	if (!target) return false;
	
	// Check if target is moving sideways by accessing entityLocation
	if (!target->entityLocation) return false;
	
	Vec3 velocity = target->entityLocation->velocity;
	float sideVelocity = std::abs(velocity.x) + std::abs(velocity.z);
	return sideVelocity > 0.1f;
}

bool FullAuto::shouldAirJumpForSafety(Entity* player) {
	if (!player) return false;
	
	Vec3 pos = *player->getPos();
	if (!player->entityLocation) return false;
	
	Vec3 velocity = player->entityLocation->velocity;
	
	// Check if falling fast (potential fall damage)
	if (velocity.y < FALL_VELOCITY_THRESHOLD) {
		// Check if close to void (y < VOID_Y_THRESHOLD)
		if (pos.y < VOID_Y_THRESHOLD) {
			return true;
		}
		
		// Estimate fall distance
		float fallDistance = std::abs(velocity.y) * FALL_DISTANCE_MULTIPLIER;
		if (fallDistance > 10.0f) {
			return true;
		}
	}
	
	return false;
}
