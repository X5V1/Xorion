#pragma once
#include "ICommand.h"

class ForceOpCommand : public IMCCommand {
public:
	ForceOpCommand();
	~ForceOpCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
