#include "NoFall.h"

NoFall::NoFall() : IModule(VK_NUMPAD3, Category::PLAYER, "Prevents you from taking falldamage") {
	// TODO: Settings system redesigned in 1.21.130
	/*mode.addEntry(EnumEntry("Vanilla", 0))
		.addEntry(EnumEntry("Mineplex", 1))
		.addEntry(EnumEntry("CubeCraft", 2))
		.addEntry(EnumEntry("Nukkit", 3))
		.addEntry(EnumEntry("AuthGroundPos", 4));
	registerEnumSetting("Mode", &mode, 0);*/
}

NoFall::~NoFall() {}

const char* NoFall::getModuleName() {
	return "NoFall";
}

void NoFall::onSendPacket(Packet* packet) {
	LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	if (localPlayer->getFallDistanceComponent()->fallDistance > 2.f && mode == 0) {
		if (packet->isInstanceOf<MovePlayerPacket>()) {
			MovePlayerPacket* movePacket = reinterpret_cast<MovePlayerPacket*>(packet);
			movePacket->onGround = true;
		}
	}
	if (mode == 4) {
		if (packet->isInstanceOf<PlayerAuthInputPacket>() && !g_Data.getLocalPlayer()->isOnGround()) {
			PlayerAuthInputPacket* authInput = reinterpret_cast<PlayerAuthInputPacket*>(packet);
			authInput->pos = closestGround;
		}
		/*if (packet->isInstanceOf<MovePlayerPacket>() && !Game.getLocalPlayer()->isOnGround()) { I don't know if this is better to have or not
			MovePlayerPacket* movePacket = reinterpret_cast<MovePlayerPacket*>(packet);
			movePacket->Position = closestGround;
		}*/
	}
}

bool NoFall::isOverVoid() {
	for (float posY = g_Data.getLocalPlayer()->getPos().y; posY > 0.0f; --posY) {
		if (!(g_Data.getLocalPlayer()->getRegion()->getBlock(Vec3(g_Data.getLocalPlayer()->getPos().x, posY, g_Data.getLocalPlayer()->getPos().z))->blockLegacy->blockId == 0)) {
			return false;
		}
	}
	return true;
};

void NoFall::onTick(GameMode* gm) {
	LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer != nullptr) {
		if (localPlayer->getFallDistanceComponent()->fallDistance > 2.f) {
			switch (mode) {
			case 0: {
				PlayerActionPacket actionPacket;
				actionPacket.action = 7;  // Respawn
				actionPacket.entityRuntimeId = localPlayer->getRuntimeIDComponent()->runtimeID;
				Game.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);
				break;
			}
			case 2: {
				localPlayer->entityLocation->velocity.y = 0.f;
				localPlayer->setPos(localPlayer->getPos().add(0, (float)0.2, 0.f));
				break;
			}
			case 3: {
				PlayerActionPacket actionPacket;
				actionPacket.action = 15;  // Open Elytra
				actionPacket.entityRuntimeId = localPlayer->getRuntimeIDComponent()->runtimeID;
				Game.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);
			}
			case 4: {
				Vec3 blockBelow = localPlayer->getPos();
				blockBelow.y -= localPlayer->getAABBShapeComponent()->size.y;
				blockBelow.y -= 0.17999f;
				while (localPlayer->getRegion()->getBlock(blockBelow)->blockLegacy->blockId == 0 && !localPlayer->getRegion()->getBlock(blockBelow)->blockLegacy->isSolid) {
					blockBelow.y -= 1.f;
					if (isOverVoid()) {
						return;
					}
				}
				blockBelow.y += 2.62001f;
				Vec3 pos = localPlayer->getPos();
				closestGround = {pos.x, blockBelow.y, pos.z};
			}
			}
		}
	}
}