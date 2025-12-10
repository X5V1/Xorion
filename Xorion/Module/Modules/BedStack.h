#pragma once

#include "Module.h"

class BedStack : public IModule {
private:
	int maxStackSize = 64;

public:
	BedStack();
	~BedStack();

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
