#pragma once
#include "Module.h"

class FontChanger : public IModule {
public:
	int fontMode = 0; // 0 = Notosans, 1 = Mojangles
	FontChanger();
	~FontChanger();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};