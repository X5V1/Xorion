#include "Blink.h"
#include "../../../SDK/Packet.h"

Blink::Blink() : IModule(0, Category::MISC, "Stops you from sending packets and then sends them in a bunch.") {
}

Blink::~Blink() {
	this->getMovePlayerPacketHolder()->clear();
	this->getPlayerAuthInputPacketHolder()->clear();
}

const char* Blink::getModuleName() {
	return "Blink";
}

void Blink::onTick(GameMode* gm) {
	gm->player->getFallDistanceComponent()->fallDistance = 0.f;
}
