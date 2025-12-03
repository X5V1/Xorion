#include "GameModeCommand.h"

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
		if (forceMode) {
			// Aggressive mode - multiple attempts to force the gamemode change
			clientMessageF("[%sXorion%s] %sAggressive gamemode change initiated...", GOLD, WHITE, YELLOW);

			for (int i = 1; i <= MAX_AGGRESSIVE_ATTEMPTS; i++) {
				Game.getLocalPlayer()->setPlayerGameType(static_cast<GameType>(gamemode));
				clientMessageF("[%sXorion%s] %sForce attempt %d/%d - Gamemode %d", GOLD, WHITE, GRAY, i, MAX_AGGRESSIVE_ATTEMPTS, gamemode);
			}

			clientMessageF("[%sXorion%s] %sGamemode %d forced! %s(%d bypass attempts completed)", GOLD, WHITE, GREEN, gamemode, GRAY, MAX_AGGRESSIVE_ATTEMPTS);
		} else {
			// Standard mode - single attempt
			Game.getLocalPlayer()->setPlayerGameType(static_cast<GameType>(gamemode));
			clientMessageF("[%sXorion%s] %sGamemode changed to %d", GOLD, WHITE, GREEN, gamemode);
			clientMessageF("[%sXorion%s] %sUse %s.gm %d force%s for aggressive bypass attempts", GOLD, WHITE, GRAY, WHITE, gamemode, GRAY);
		}
	} else {
		clientMessageF("[%sXorion%s] %sInvalid gamemode! Use 0-6", GOLD, WHITE, RED);
	}

	return true;
}
