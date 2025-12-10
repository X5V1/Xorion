#pragma once

#include "Module.h"

class Replenish : public IModule {
private:
	int delay = 0;
	int lastSlot = -1;
	int lastItemId = -1;
	int lastAuxValue = -1;
	
public:
	Replenish();
	~Replenish();

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};
