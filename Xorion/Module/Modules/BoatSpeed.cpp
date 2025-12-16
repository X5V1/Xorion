#include "BoatSpeed.h"

#include <math.h>

#include "../../../Memory/GameData.h"
#include "../../../SDK/GameSettingsInput.h"

BoatSpeed::BoatSpeed() : IModule(0, Category::MOVEMENT, "Boat speed matches blue ice momentum regardless of surface.") {
	registerFloatSetting("Speed", &targetSpeed, targetSpeed, 0.1f, 5.0f);
}

BoatSpeed::~BoatSpeed() {}

const char* BoatSpeed::getModuleName() {
	return "BoatSpeed";
}

void BoatSpeed::onTick(GameMode* gm) {
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

	float forward = 0.f;
	float strafe = 0.f;

	if (GameData::isKeyDown(*input->forwardKey)) forward += 1.f;
	if (GameData::isKeyDown(*input->backKey)) forward -= 1.f;
	if (GameData::isKeyDown(*input->rightKey)) strafe += 1.f;
	if (GameData::isKeyDown(*input->leftKey)) strafe -= 1.f;

	if (forward == 0.f && strafe == 0.f)
		return;

	auto rot = gm->player->getActorRotationComponent();
	if (rot == nullptr)
		return;

	float calcYaw = (rot->rot.y + 90.0f) * RAD_DEG;
	float c = cosf(calcYaw);
	float s = sinf(calcYaw);

	Vec2 moveVec2D{ forward, -strafe };
	moveVec2D = { moveVec2D.x * c - moveVec2D.y * s, moveVec2D.x * s + moveVec2D.y * c };

	boat->entityLocation->velocity.x = moveVec2D.x * targetSpeed;
	boat->entityLocation->velocity.z = moveVec2D.y * targetSpeed;
}
