#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class NoLava : public IModule {
public:
	NoLava();
	~NoLava();

	const char* getModuleName() override;
	void onTick(GameMode* gm) override;
	void onDisable() override;

private:
	void clearFire(Entity* ent);
};
