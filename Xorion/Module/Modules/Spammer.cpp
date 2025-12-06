#include "Spammer.h"
#include "../../../Memory/GameData.h"
#include "../../../Utils/Utils.h"
#include "../../../SDK/Packet.h"

Spammer::Spammer() : IModule(0, Category::MISC, "Spams a message in a specified delay.") {
	registerIntSetting("Delay", &delay, delay, 1, 10);
	registerIntSetting("Length", &length, length, 1, 60);
	registerBoolSetting("Bypass", &bypass, bypass);
}

Spammer::~Spammer() {
}

const char* Spammer::getModuleName() {
	return "Spammer";
}

void Spammer::onTick(GameMode* gm) {
	Odelay++;
	if (Odelay > delay * 20) {
		TextPacket textPacket;
	textPacket.message.setText(bypass ? (message + " | " + Utils::randomString(length)) : message);
	textPacket.sourceName.setText(g_Data.getLocalPlayer()->getNameTag()->getText());
	// TODO: getUserId() not available in new API
	// textPacket.xboxUserId = std::to_string(g_Data.getLocalPlayer()->getUserId());
	textPacket.xboxUserId.setText("0"); // Stub - use setText for TextHolder
	Game.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
		Odelay = 0;
	}
}
