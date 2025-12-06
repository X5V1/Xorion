#include "RelativeTeleportCommand.h"
#include "../../../Utils/TextFormat.h"
#include "../../../Utils/HMath.h"

RelativeTeleportCommand::RelativeTeleportCommand() : IMCCommand("relativeteleport", "Teleports to coordinates relative to the Player", "<X> <Y> <Z>") {
	registerAlias("reltp");
	registerAlias("rtp");
	registerAlias("vclip");
}

RelativeTeleportCommand::~RelativeTeleportCommand() {
}

bool RelativeTeleportCommand::execute(std::vector<std::string>* args) {
	assertTrue(g_Data.getLocalPlayer() != nullptr);
	assertTrue(args->size() >= 4);

	Vec3 pPos = g_Data.getLocalPlayer()->getPos();

	Vec3 pos;
	pos.x = assertFloat(args->at(1)) + pPos.x;
	pos.y = assertFloat(args->at(2)) + pPos.y;
	pos.z = assertFloat(args->at(3)) + pPos.z;

	Game.getLocalPlayer()->setPos(pos);
	clientMessageF("%sTeleported!", GREEN);
	return true;
}