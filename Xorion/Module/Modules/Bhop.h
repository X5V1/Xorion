#pragma once

#include "..\ModuleManager.h"
#include "Module.h"

class Bhop : public IModule {
private:
	float speed = 0.325f;
	bool hive = false;
	float speedFriction = 0.65f;
	float cachedStepHeignt = 0.5f;
public:
	Bhop();
	~Bhop();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	// TODO: onMove removed from IModule in 1.21.123
	//virtual void onMove(MoveInputHandler* input) override;
	virtual void onEnable() {
		auto player = Game.getLocalPlayer();
		if (player == nullptr)
			return;
		else
			cachedStepHeignt = player->getMaxAutoStepComponent()->stepHeight;
	};
	virtual void onDisable() {
		auto player = Game.getLocalPlayer();
		if (player == nullptr)
			return;
		else
			player->getMaxAutoStepComponent()->stepHeight = cachedStepHeignt;
	};
};
