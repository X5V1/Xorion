#include "EjectCommand.h"

EjectCommand::EjectCommand() : IMCCommand("eject", "Removes the cheat from the game.", "") {
	registerAlias("uninject");
}

EjectCommand::~EjectCommand() {
}

bool EjectCommand::execute(std::vector<std::string>* args) {
	g_Data.setTerminate(true);
	return true;
}
