#include "ImpersonateCommand.h"

#include "../../Module/ModuleManager.h"
#include "../../Module/Modules/Impersonator.h"
#include "../../../Utils/TextFormat.h"

ImpersonateCommand::ImpersonateCommand() : IMCCommand("impersonate", "Set or remove player to impersonate in chat", "<set/remove> <player_name>") {
	registerAlias("imp");
}

ImpersonateCommand::~ImpersonateCommand() {
}

bool ImpersonateCommand::execute(std::vector<std::string>* args) {
	assertTrue(Game.getLocalPlayer() != nullptr);
	assertTrue(args->size() >= 2);

	std::string option = args->at(1);
	std::transform(option.begin(), option.end(), option.begin(), ::tolower);

	auto impersonatorMod = moduleMgr->getModule<Impersonator>();
	if (impersonatorMod == nullptr) {
		clientMessageF("%sCould not find Impersonator module!", RED);
		return true;
	}

	if (option == "set") {
		assertTrue(args->size() >= 3);
		
		// Combine all remaining args as the player name (in case of spaces)
		std::ostringstream os;
		for (size_t i = 2; i < args->size(); i++) {
			if (i > 2)
				os << " ";
			os << args->at(i);
		}
		std::string playerName = os.str();
		
		impersonatorMod->setTargetPlayerName(playerName);
		clientMessageF("%sImpersonator target set to %s%s%s!", GREEN, GRAY, playerName.c_str(), GREEN);
		clientMessageF("%sEnable the Impersonator module to start impersonating!", GREEN);
		return true;
	} else if (option == "remove") {
		impersonatorMod->clearTarget();
		clientMessageF("%sImpersonator target removed!", GREEN);
		return true;
	}

	return false;
}
