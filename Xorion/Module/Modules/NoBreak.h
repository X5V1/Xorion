#pragma once

#include "Module.h"

class NoBreak : public IModule {
public:
	NoBreak();
	~NoBreak();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
