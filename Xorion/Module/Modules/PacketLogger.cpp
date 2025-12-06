#include "PacketLogger.h"
#include "../../../Memory/GameData.h"
#include "../../../Utils/Logger.h"

PacketLogger::PacketLogger() : IModule(0x0, Category::WORLD, "Packet Logger") {
	// TODO: Settings system redesigned
}

PacketLogger::~PacketLogger() {}

const char* PacketLogger::getModuleName() {
return "PacketLogger";
}

void PacketLogger::onSendPacket(Packet* packet) {
	// TODO: Implement packet logging logic
	if (!packet) return;
	// logF("[PacketLogger] Packet sent: %p", packet);
}
