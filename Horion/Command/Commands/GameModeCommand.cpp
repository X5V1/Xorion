#include "GameModeCommand.h"

GameModeCommand::GameModeCommand() : IMCCommand("gamemode", "Unleash your true power - dominate reality by shifting dimensions", "<number>") {
	registerAlias("gm");
}

GameModeCommand::~GameModeCommand() {
}

bool GameModeCommand::execute(std::vector<std::string>* args) {
	assertTrue(Game.getLocalPlayer() != nullptr);
	assertTrue(args->size() > 1);

	int gamemode = assertInt(args->at(1));

	if (gamemode >= 0 && gamemode <= 6) {
		Game.getLocalPlayer()->setPlayerGameType(static_cast<GameType>(gamemode));

		// Aggressive and powerful messaging based on gamemode
		switch (gamemode) {
		case 0:  // Survival
			clientMessageF("[%sXorion%s] %s%sREALITY BENDS TO YOUR WILL!", GOLD, WHITE, RED, BOLD);
			clientMessageF("[%sXorion%s] %sSurvival Mode %sENGAGED%s - Face the world like a %sTRUE WARRIOR!",
						   GOLD, WHITE, GREEN, YELLOW, WHITE, RED);
			break;
		case 1:  // Creative
			clientMessageF("[%sXorion%s] %s%sGODMODE ACTIVATED!", GOLD, WHITE, LIGHT_PURPLE, BOLD);
			clientMessageF("[%sXorion%s] %sCreative Mode %sUNLEASHED%s - You are now %sUNSTOPPABLE!",
						   GOLD, WHITE, GREEN, YELLOW, WHITE, LIGHT_PURPLE);
			clientMessageF("[%sXorion%s] %sInfinite power courses through your veins!", GOLD, WHITE, AQUA);
			break;
		case 2:  // Adventure
			clientMessageF("[%sXorion%s] %s%sLEGEND MODE INITIATED!", GOLD, WHITE, GOLD, BOLD);
			clientMessageF("[%sXorion%s] %sAdventure Mode %sACTIVATED%s - Your %sEPIC QUEST%s begins!",
						   GOLD, WHITE, GREEN, YELLOW, WHITE, GOLD, WHITE);
			break;
		case 3:  // Spectator
			clientMessageF("[%sXorion%s] %s%sSHADOW REALM ACCESSED!", GOLD, WHITE, DARK_PURPLE, BOLD);
			clientMessageF("[%sXorion%s] %sSpectator Mode %sENABLED%s - You are now %sBEYOND MORTAL SIGHT!",
						   GOLD, WHITE, GREEN, YELLOW, WHITE, DARK_PURPLE);
			clientMessageF("[%sXorion%s] %sPhase through reality like a %sPHANTOM!", GOLD, WHITE, GRAY, DARK_AQUA);
			break;
		default:
			clientMessageF("[%sXorion%s] %s%sDIMENSION SHIFT COMPLETE!", GOLD, WHITE, BLUE, BOLD);
			clientMessageF("[%sXorion%s] %sGamemode %d %sACTIVATED%s - %sRAW POWER UNLEASHED!",
						   GOLD, WHITE, GREEN, gamemode, YELLOW, WHITE, RED);
			break;
		}
	} else {
		clientMessageF("[%sXorion%s] %s%sERROR: POWER OVERFLOW!", GOLD, WHITE, RED, BOLD);
		clientMessageF("[%sXorion%s] %sInvalid GameMode! %sThe universe rejects your command!", GOLD, WHITE, RED, DARK_RED);
		clientMessageF("[%sXorion%s] %sUse 0-6 to channel your power correctly!", GOLD, WHITE, YELLOW);
	}

	return true;
}
