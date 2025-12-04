#include "CubeGlide.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/Vec2.h"
#include "../../../SDK/GameMode.h"
#include "../../../SDK/Packet.h"

CubeGlide::CubeGlide() : IModule(0, Category::MOVEMENT, "Glide on Cubecraft") {
	registerFloatSetting("Speed", &this->speed, this->speed, 1.f, 3.f);
}

CubeGlide::~CubeGlide() {
}

const char* CubeGlide::getModuleName() {
	return "CubeGlide";
}

// TODO: isFlashMode removed from IModule in 1.21.123
/*
bool CubeGlide::isFlashMode() {
	return true;
}
*/

void CubeGlide::onEnable() {
	g_Data.getLocalPlayer()->setPos(g_Data.getLocalPlayer()->getPos().add(vec3_t(0, 1, 0)));
}

void CubeGlide::onTick(C_GameMode* gm) {
	float calcYaw = (gm->player->getActorHeadRotationComponent()->rot.y + 90) * (PI / 180);

	gameTick++;

	vec3_t pos = g_Data.getLocalPlayer()->getPos();
	pos.y += 1.3f;
	MovePlayerPacket packet1(g_Data.getLocalPlayer(), pos);
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&packet1);
	pos.y -= 1.3f;
	MovePlayerPacket packet2(g_Data.getLocalPlayer(), pos);
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&packet2);

	vec3_t moveVec;
	moveVec.x = cos(calcYaw) * speed;
	moveVec.z = sin(calcYaw) * speed;

	gm->player->lerpMotion(moveVec);

	if (gameTick >= 5) {
		gameTick = 0;
		float yaw = gm->player->getActorHeadRotationComponent()->rot.y * (PI / 180);
		float length = 4.f;

		float x = -sin(yaw) * length;
		float z = cos(yaw) * length;

		gm->player->setPos(pos.add(vec3_t(x, 0.5f, z)));
	}
}

void CubeGlide::onDisable() {
	// TODO: Entity velocity not directly accessible
	// g_Data.getLocalPlayer()->velocity = vec3_t(0, 0, 0);
}
