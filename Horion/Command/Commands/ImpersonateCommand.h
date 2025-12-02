#pragma once
#include "ICommand.h"
#include <sstream>

class ImpersonateCommand : public IMCCommand {
public:
	ImpersonateCommand();
	~ImpersonateCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
