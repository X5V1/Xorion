#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class BoatSpeed : public IModule {
private:
	float targetSpeed = 2.0f;

public:
	BoatSpeed();
	~BoatSpeed();

	const char* getModuleName() override;
	void onTick(GameMode* gm) override;
};
