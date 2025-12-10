#pragma once

#include "../../DrawUtils.h"
#include "Module.h"

class NoFall : public IModule {
public:
	// TODO: SettingEnum removed in 1.21.124 - using dummy default
	//SettingEnum mode = SettingEnum(this);
	int mode = 0; // Dummy variable
	Vec3 closestGround;
	bool isOverVoid();
	NoFall();
	~NoFall();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onSendPacket(Packet* packet) override;
	virtual void onTick(GameMode* gm) override;
};