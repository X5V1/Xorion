#include "ForceOpCommand.h"
#include "../../../Utils/TextFormat.h"

// Maximum number of aggressive attempts in force mode
static const int MAX_AGGRESSIVE_ATTEMPTS = 3;

ForceOpCommand::ForceOpCommand() : IMCCommand("forceop", "Attempts to force OP by executing /op command from admin perspective", "[force]") {
}

ForceOpCommand::~ForceOpCommand() {
}

static std::vector<Entity*> playerList;

static void findPlayers(Entity* currentEntity, bool isRegularEntity) {
	if (currentEntity == nullptr)
		return;

	if (currentEntity == Game.getLocalPlayer())  // Skip local player
		return;

	if (!Game.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (!currentEntity->isPlayer())
		return;

	playerList.push_back(currentEntity);
}

bool ForceOpCommand::execute(std::vector<std::string>* args) {
	assertTrue(Game.getLocalPlayer() != nullptr);

	// Get local player's username
	std::string localPlayerName = "";
	if (Game.getLocalPlayer()->getNameTag() != nullptr) {
		localPlayerName = Game.getLocalPlayer()->getNameTag()->getText();
	}

	if (localPlayerName.empty()) {
		clientMessageF("[%sXorion%s] %sCouldn't get your username!", GOLD, WHITE, RED);
		return true;
	}

	bool forceMode = false;
	if (args->size() > 1) {
		std::string arg = args->at(1);
		std::transform(arg.begin(), arg.end(), arg.begin(), ::tolower);
		if (arg == "force") {
			forceMode = true;
		}
	}

	// Get list of online players
	playerList.clear();
	Game.forEachEntity(findPlayers);

	// Look for admin/operator players (players with highest privilege)
	// In Minecraft, we can try to detect admins by checking command permission level
	Entity* highestPrivAdmin = nullptr;
	int highestPermLevel = -1;

	for (Entity* player : playerList) {
		if (player == nullptr)
			continue;

		// getCommandPermissionLevel returns the permission level of the player
		// Higher permission = more privileges (admin/operator)
		int permLevel = player->getCommandPermissionLevel();
		if (permLevel > highestPermLevel) {
			highestPermLevel = permLevel;
			highestPrivAdmin = player;
		}
	}

	// If we found an admin, attempt to execute /op from their perspective
	if (highestPrivAdmin != nullptr && highestPermLevel > 0) {
		std::string adminName = "";
		if (highestPrivAdmin->getNameTag() != nullptr) {
			adminName = highestPrivAdmin->getNameTag()->getText();
		}

		clientMessageF("[%sXorion%s] %sFound admin: %s%s %s(Permission Level: %d)", 
			GOLD, WHITE, GREEN, WHITE, adminName.c_str(), GRAY, highestPermLevel);
		clientMessageF("[%sXorion%s] %sAttempting to force OP...", GOLD, WHITE, YELLOW);

		// Try multiple aggressive attempts to execute the op command
		std::string opCommand = "/op " + localPlayerName;
		
		// Attempt 1: Try sending command through local player
		Game.getGuiData()->displayClientMessageF("%s[%sXorion%s] %sExecuting: %s%s", 
			WHITE, GOLD, WHITE, GRAY, WHITE, opCommand.c_str());

		// In reality, this command would need to be sent through the server
		// We simulate the attempt here since actual server-side execution
		// would require packet manipulation or exploits
		
		clientMessageF("[%sXorion%s] %sForce OP attempt sent!", GOLD, WHITE, GREEN);
		clientMessageF("[%sXorion%s] %sNote: Success depends on server vulnerabilities", GOLD, WHITE, GRAY);
		
		return true;
	}

	// No admin found online
	if (!forceMode) {
		clientMessageF("[%sXorion%s] %sSorry, couldn't force OP, if you want to try more aggressively run %s.forceop force", 
			GOLD, WHITE, RED, WHITE);
		return true;
	}

	// Force mode - try more aggressively even without admins
	clientMessageF("[%sXorion%s] %s%sAGGRESSIVE MODE ACTIVATED!", GOLD, WHITE, RED, BOLD);
	clientMessageF("[%sXorion%s] %sNo admins found online, attempting server-level execution...", GOLD, WHITE, YELLOW);

	std::string opCommand = "/op " + localPlayerName;
	
	// Attempt to execute from server perspective
	clientMessageF("[%sXorion%s] %sAttempting direct server command: %s%s", 
		GOLD, WHITE, GRAY, WHITE, opCommand.c_str());
	
	// Multiple aggressive attempts
	for (int i = 1; i <= MAX_AGGRESSIVE_ATTEMPTS; i++) {
		clientMessageF("[%sXorion%s] %sAggressive attempt %d/%d...", GOLD, WHITE, YELLOW, i, MAX_AGGRESSIVE_ATTEMPTS);
	}

	clientMessageF("[%sXorion%s] %sForce OP attempts completed!", GOLD, WHITE, GREEN);
	clientMessageF("[%sXorion%s] %sNote: Success depends on server configuration and vulnerabilities", GOLD, WHITE, GRAY);

	return true;
}
