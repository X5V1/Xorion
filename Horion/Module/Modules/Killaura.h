#pragma once

#include "Module.h"
#include "../ModuleManager.h"
#include "../../../Utils/Target.h"

class Killaura : public IModule
{
private:
	int delay = 0;
	int Odelay = 0;
	bool autoweapon = false;
	void findWeapon();
	// TODO: SettingEnum removed in 1.21.123 - using dummy defaults
	//SettingEnum mode{this};
	//SettingEnum rotationMode{this};
	int mode = 0; // Dummy: 0=Single, 1=Multi
	int rotationMode = 2; // Dummy: 0=None, 1=Silent, 2=Strafe, 3=Normal

public:
	bool targetListEmpty = false;
	Vec2 angle;
	bool isMobAura = false;
	bool hurttime = true;
	float range = 6;


	Killaura();
	~Killaura();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onSendPacket(Packet* packet) override;
	virtual void onPlayerTick(Player* player) override;
};