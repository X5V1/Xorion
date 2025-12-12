#pragma once

#include "Module.h"

class Fly : public IModule {
private:
	float horizontalSpeed = 1.5f;
	float verticalSpeed = 0.5;

	int gameTick = 0;
	// TODO: SettingEnum removed - using dummy default mode
	int mode = 0;

public:
	Fly();
	~Fly();

	// TODO: SettingEnum removed in 1.21.130
	//SettingEnum mode = SettingEnum(this);

	// Inherited via IModule
	virtual void onEnable() override;
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onDisable() override;
	// TODO: onMove removed from IModule in 1.21.130
	//virtual void onMove(MoveInputHandler* input) override;
};
