#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class BoatFly : public IModule {
private:
	float verticalSpeed = 0.35f;

public:
	BoatFly();
	~BoatFly();

	const char* getModuleName() override;
	void onTick(GameMode* gm) override;
};
