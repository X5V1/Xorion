#pragma once

#include "Module.h"

class Reducer : public IModule {
public:
	Reducer();
	~Reducer();

	int mode = 0; // 0 = Jump, 1 = Sneak, 2 = JumpReset

	int hitCount = 0;
	int ticks = 0;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual const char* getRawModuleName() override;
	virtual void onEnable() override;
	virtual void onTick(GameMode* gm) override;
};
