#include "Derp.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/Packet.h"

Derp::Derp() : IModule(0, Category::MISC, "lol you stupid.") {
	registerBoolSetting("ihaveastroke", &epicStroke, epicStroke);
	registerBoolSetting("packet mode", &packetMode, packetMode);
}

Derp::~Derp() {
}

const char* Derp::getModuleName() {
	return "Derp";
}

void Derp::onTick(GameMode* gm) {
	if (packetMode) {
		MovePlayerPacket packet(Game.getLocalPlayer(), Game.getLocalPlayer()->getPos());
		if (epicStroke) {
			packet.pitch = (float)(rand() % 360);
			packet.yaw = (float)(rand() % 360);
		} else {
			packet.pitch = (float)(counter % 360);
			packet.yaw = (float)(counter % 360);
		}
		Game.getClientInstance()->loopbackPacketSender->sendToServer(&packet);
	} else {
		if (epicStroke) {
			gm->player->getActorHeadRotationComponent()->rot.x = (float)(rand() % 360);
			gm->player->getMobBodyRotationComponent()->bodyRot = (float)(rand() % 360);
		} else {
			gm->player->getActorHeadRotationComponent()->rot.x = (float)(counter % 360);
			gm->player->getMobBodyRotationComponent()->bodyRot = (float)(counter % 360);
		}
	}

	if (counter < 360)
		counter++;
	else
		counter = 0;
}
