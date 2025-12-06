#include "DeviceIDCommand.h"
#include "../../../Utils/TextFormat.h"
#include "../../../Utils/Utils.h"
#include "../../../SDK/Player.h"

DeviceIDCommand::DeviceIDCommand() : IMCCommand("deviceid", "Displays the device id from you or another player", "[playername]") {
}

DeviceIDCommand::~DeviceIDCommand() {
}

static std::vector<Entity*> targetList724;
void findEntity84(Entity* currentEntity, bool isRegularEntity) {
	if (currentEntity == nullptr)
		return;

	if (currentEntity == Game.getLocalPlayer())  // Skip Local player
		return;

	if (!((Entity*)Game.getLocalPlayer())->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (!currentEntity->isPlayer())
		return;

	targetList724.push_back(currentEntity);
}

bool DeviceIDCommand::execute(std::vector<std::string>* args) {
	if (args->size() < 2) {
		auto player = Game.getLocalPlayer();

		// TODO: getDeviceNameLocal() not available
		clientMessageF("%sYour device: %s%s", GREEN, GRAY, "Unknown"); // player->getDeviceNameLocal()
	}
	else {
		targetList724.clear();
		Game.forEachEntity(findEntity84);

		if (!targetList724.empty())
			return true;

		std::string nameOfPlayer = args->at(1);
		assertTrue(!nameOfPlayer.empty());
		std::string nameOfPlayerLower = std::string(nameOfPlayer);
		std::transform(nameOfPlayerLower.begin(), nameOfPlayerLower.end(), nameOfPlayerLower.begin(), ::tolower);
		nameOfPlayerLower = Utils::sanitize(nameOfPlayerLower);

		size_t listSize = targetList724.size();

		if (listSize > 5000) {
			return true;
		}

		Player* tar = nullptr;

		// Loop through all our players and retrieve their information
		for (size_t i = 0; i < listSize; i++) {
			Entity* currentEntity = targetList724.at(i);

			if (currentEntity == nullptr)
				break;

			if (currentEntity == Game.getLocalPlayer())  // Skip Local player
				continue;

			if (currentEntity->getNameTag() == nullptr)
				continue;

			std::string name(currentEntity->getNameTag()->getText());

			std::transform(name.begin(), name.end(), name.begin(), ::tolower);

			if (name.find(nameOfPlayerLower) == std::string::npos)
				continue;

			tar = reinterpret_cast<Player*>(currentEntity);
			break;
		}

		if (tar == nullptr) {
			clientMessageF("[%sXorion%s] %sCouldn't find player: %s!", GOLD, WHITE, RED, nameOfPlayer.c_str());
			return true;
		}

		clientMessageF("%sDevice from %s%s%s: %s%s", GREEN, GRAY, ((Player*)tar)->getNameTag()->getText(), GREEN, GRAY, ((Player*)tar)->getDeviceName());
	}

	return true;
}
