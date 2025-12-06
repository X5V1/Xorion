#include "AirStuck.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/GameMode.h"

AirStuck::AirStuck() : IModule(0, Category::MOVEMENT, "Become stuck.") {
}

AirStuck::~AirStuck() {
}

const char* AirStuck::getModuleName() {
	return ("AirStuck");
}

void AirStuck::onTick(C_GameMode* gm) {
	// gm->player->velocity = vec3_t(0, 0, 0); // TODO: Implement velocity zeroing
}