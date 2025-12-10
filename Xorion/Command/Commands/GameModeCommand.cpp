#include "GameModeCommand.h"
#include "../../../Utils/TextFormat.h"
#include "../../../SDK/GameType.h"

// Maximum aggressive retry attempts for gamemode change
static const int MAX_AGGRESSIVE_ATTEMPTS = 3;

GameModeCommand::GameModeCommand() : IMCCommand("gamemode", "Aggressively forces gamemode change with multiple bypass attempts", "<number> [force]") {
	registerAlias("gm");
}

GameModeCommand::~GameModeCommand() {
}

bool GameModeCommand::execute(std::vector<std::string>* args) {
	assertTrue(Game.getLocalPlayer() != nullptr);
	assertTrue(args->size() > 1);

	int gamemode = assertInt(args->at(1));

	// Check for force mode
	bool forceMode = false;
	if (args->size() > 2) {
		std::string arg = args->at(2);
		std::transform(arg.begin(), arg.end(), arg.begin(), ::tolower);
		if (arg == "force") {
			forceMode = true;
		}
	}

	if (gamemode >= 0 && gamemode <= 6) {
		// TODO: setPlayerGameType not available in 1.21.124
		// Game.getLocalPlayer()->setPlayerGameType(static_cast<GameType>(gamemode));
		clientMessageF("[%sXorion%s] %sGameMode change not available in 1.21.124!", GOLD, WHITE, RED);
	} else {
		clientMessageF("[%sXorion%s] %sInvalid gamemode! Use 0-6", GOLD, WHITE, RED);
	}

	return true;
}
