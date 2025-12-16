#include "BoatFly.h"

#include "../../../Memory/GameData.h"
#include "../../../SDK/GameSettingsInput.h"

BoatFly::BoatFly() : IModule(0, Category::MOVEMENT, "Fly while riding a boat. Jump to go up, shift to go down.") {
	registerFloatSetting("Vertical Speed", &verticalSpeed, verticalSpeed, 0.05f, 1.0f);
}

BoatFly::~BoatFly() {}

const char* BoatFly::getModuleName() {
	return "BoatFly";
}

void BoatFly::onTick(GameMode* gm) {
	if (gm == nullptr || gm->player == nullptr)
		return;

	auto movement = gm->player->getMovementProxy();
	if (movement == nullptr || !movement->isRiding())
		return;

	auto boat = reinterpret_cast<Entity*>(movement->getVehicle());
	if (boat == nullptr || boat->entityLocation == nullptr)
		return;

	auto input = Game.getClientInstance()->getGameSettingsInput();
	if (input == nullptr)
		return;

	float vertical = 0.0f;
	if (GameData::isKeyDown(*input->spaceBarKey))
		vertical += verticalSpeed;
	if (GameData::isKeyDown(*input->sneakKey))
		vertical -= verticalSpeed;

	if (vertical != 0.0f) {
		boat->entityLocation->velocity.y = vertical;
	}
}
