#include "Twerk.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/ClientInstance.h"

Twerk::Twerk() : IModule(0x0, Category::MOVEMENT, "Crouch Spam.") {
	registerFloatSetting("Delay", &delay, delay, 0.05f, 10);
}

Twerk::~Twerk() {
}

const char* Twerk::getModuleName() {
	return "Twerk";
}

void Twerk::onTick(GameMode* gm) {
	Odelay++;
	auto ci = Game.getClientInstance();
	if (ci && ci->minecraft && ci->minecraft->simTimer) {
		if (Odelay > delay * *ci->minecraft->simTimer) {
			auto moveInput = ci->getMoveTurnInput();
			if (moveInput) {
				moveInput->isSneakDown = !moveInput->isSneakDown;
			}
			Odelay = 0;
		}
	}
}