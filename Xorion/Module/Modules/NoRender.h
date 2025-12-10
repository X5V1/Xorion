#pragma once

#include "Module.h"

class NoRender : public IModule {
public:
	NoRender();
	~NoRender();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
