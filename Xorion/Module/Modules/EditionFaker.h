#pragma once

#include "Module.h"

class EditionFaker : public IModule {
public:
	// TODO: SettingEnum removed in 1.21.130
	//SettingEnum edition = SettingEnum(this);
	
	EditionFaker();
	~EditionFaker();

	int getFakedEditon();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
