#include "NoLava.h"

#include "../../../Memory/GameData.h"

NoLava::NoLava() : IModule(0, Category::PLAYER, "Nothing burns in lava, including boats.") {
}

NoLava::~NoLava() {}

const char* NoLava::getModuleName() {
	return "NoLava";
}

void NoLava::clearFire(Entity* ent) {
	if (ent == nullptr)
		return;

	ent->setStatusFlag(ActorFlags::ONFIRE, false);
	ent->setStatusFlag(ActorFlags::FIRE_IMMUNE, true);
}

void NoLava::onTick(GameMode* gm) {
	if (gm == nullptr || gm->player == nullptr)
		return;

	clearFire(gm->player);

	auto movement = gm->player->getMovementProxy();
	if (movement != nullptr && movement->isRiding()) {
		auto vehicle = reinterpret_cast<Entity*>(movement->getVehicle());
		clearFire(vehicle);
	}
}

void NoLava::onDisable() {
	if (auto lp = Game.getLocalPlayer(); lp != nullptr) {
		lp->setStatusFlag(ActorFlags::FIRE_IMMUNE, false);
		lp->setStatusFlag(ActorFlags::ONFIRE, false);

		if (auto movement = lp->getMovementProxy(); movement != nullptr && movement->isRiding()) {
			if (auto vehicle = reinterpret_cast<Entity*>(movement->getVehicle())) {
				vehicle->setStatusFlag(ActorFlags::FIRE_IMMUNE, false);
				vehicle->setStatusFlag(ActorFlags::ONFIRE, false);
			}
		}
	}
}
