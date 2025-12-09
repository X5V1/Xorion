#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class Zoom : public IModule {
private:
	bool zooming = false;
	float target = 0.f;
	bool smooth = false;
	
public:
	float strength = 0.5f;

	Zoom();
	~Zoom();

	virtual const char* getModuleName() override;
	// TODO: isFlashMode removed from IModule in 1.21.124
	//bool isFlashMode() override;
	void onEnable() override;
	void onDisable() override;
	void onLevelRender() override;
};