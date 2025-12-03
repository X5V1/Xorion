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
	SettingEnum mode{this};
	SettingEnum rotationMode{this};

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