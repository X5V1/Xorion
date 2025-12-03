#include "InfiniteTreasureAura.h"

InfiniteTreasureAura::InfiniteTreasureAura() : IModule(0x0, COMBAT) // <-- keybind
{
	this->registerBoolSetting("multiaura", &this->isMulti, this->isMulti);
	this->registerBoolSetting("Target Players", &this->player, this->player);
	this->registerBoolSetting("Target Treasure", &this->treasure, this->treasure);
	this->registerFloatSetting("range", &this->range, this->range, 15, 100);
	this->registerIntSetting("delay", &this->delay, this->delay, 15, 20);
}


InfiniteTreasureAura::~InfiniteTreasureAura()
{
}

const char* InfiniteTreasureAura::getModuleName()
{
	return ("InfiniteTreasure");
}

static std::vector <C_Entity*> targetList0;

void findEntitiess(C_Entity* currentEntity1, bool isRegularEntitie) {
	static InfiniteTreasureAura* InfiniteReachMod1 = static_cast<InfiniteTreasureAura*>(moduleMgr->getModule<InfiniteTreasureAura>());
	if (InfiniteReachMod1 == 0)
		InfiniteReachMod1 = static_cast<InfiniteTreasureAura*>(moduleMgr->getModule<InfiniteTreasureAura>());
	else {
		if (currentEntity1 == g_Data.getLocalPlayer()) // Skip Local player
			return;

		if (currentEntity1 == 0)
			return;

		if (currentEntity1->timeSinceDeath > 0)
			return;

		if (FriendList::findPlayer(currentEntity1->getNameTag()->getText())) // Skip Friend
			return;
		if (!((currentEntity1->width == 0.6f && currentEntity1->height == 1.8f) || (currentEntity1->width == 0.9f && currentEntity1->height == 0.9f) || (currentEntity1->width == 2.7f && currentEntity1->height == 2.7f))) return;

		if (!InfiniteReachMod1->player && (currentEntity1->width == 0.6f && currentEntity1->height == 1.8f)) return;
		if (!InfiniteReachMod1->treasure && ((currentEntity1->width == 0.9f && currentEntity1->height == 0.9f) || (currentEntity1->width == 0.9f && currentEntity1->height == 0.9f))) return;

		float dist = (*currentEntity1->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

		if (dist < InfiniteReachMod1->range)
		{
			targetList0.push_back(currentEntity1);
		}
	}
}

void InfiniteTreasureAura::onTick(C_GameMode* gm)
{
	if (!g_Data.isInGame())
		return;

	//Loop through all our players and retrieve their information
	targetList0.clear();

	g_Data.forEachEntity(findEntitiess);
	Odelay++;
	float calcYaw = (gm->player->yaw + 90) * (PI / 180);
	float calcPitch = (gm->player->pitch) * -(PI / 180);

	if (targetList0.size() > 0 && Odelay >= delay)
	{
		g_Data.getLocalPlayer()->swingArm();

		float calcYaw = (gm->player->yaw + 90) * (PI / 180);
		float calcPitch = (gm->player->pitch) * -(PI / 180);

		float teleportX = cos(calcYaw) * cos(calcPitch) * 3.5f;
		float teleportZ = sin(calcYaw) * cos(calcPitch) * 3.5f;
		C_MovePlayerPacket* teleportPacket = nullptr;

		if (strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "mco.cubecraft.net") == 0)
		{
			vec3_t pos = *g_Data.getLocalPlayer()->getPos();
			C_MovePlayerPacket* oof = new C_MovePlayerPacket(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(oof);
			delete oof;
			pos.y += 0.35f;
			oof = new C_MovePlayerPacket(g_Data.getLocalPlayer(), pos);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(oof);
			delete oof;
		}

		// Attack all entitys in targetList
		if (isMulti) {
			for (int i = 0; i < targetList0.size(); i++)
			{
				vec3_t pos = *targetList0[i]->getPos();
				teleportPacket = new C_MovePlayerPacket(g_Data.getLocalPlayer(), vec3_t(pos.x - teleportX, pos.y, pos.z - teleportZ));
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(teleportPacket);
				g_Data.getGameMode()->attack(targetList0[i]);
				delete teleportPacket;
				teleportPacket = new C_MovePlayerPacket(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(teleportPacket);
				delete teleportPacket;
			}
		}
		else
		{
			vec3_t pos = *targetList0[0]->getPos();
			teleportPacket = new C_MovePlayerPacket(g_Data.getLocalPlayer(), vec3_t(pos.x - teleportX, pos.y, pos.z - teleportZ));
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(teleportPacket);
			g_Data.getGameMode()->attack(targetList0[0]);
			delete teleportPacket;
			teleportPacket = new C_MovePlayerPacket(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(teleportPacket);
			delete teleportPacket;
		}
		Odelay = 0;
	}
}