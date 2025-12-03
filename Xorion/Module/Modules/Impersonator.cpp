#include "Impersonator.h"
#include "../../../SDK/Packet.h"
#include "../../../Memory/GameData.h"

Impersonator::Impersonator() : IModule(0, Category::MISC, "Impersonate another player's username in chat.") {
}

Impersonator::~Impersonator() {
}

const char* Impersonator::getModuleName() {
	return "Impersonator";
}

void Impersonator::onSendPacket(Packet* packet) {
	if (packet == nullptr) return;
	if (!hasTarget()) return;

	// Check if this is a TextPacket (chat message)
	if (packet->isInstanceOf<TextPacket>()) {
		TextPacket* textPacket = static_cast<TextPacket*>(packet);
		
		// Modify the source name to impersonate the target player
		textPacket->sourceName.setText(targetPlayerName);
	}
}
