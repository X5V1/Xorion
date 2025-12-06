#pragma once

#include "Module.h"

// Forward declaration
struct MovePlayerPacket;

class Jetpack : public IModule {
private:
	int delay = 0;
	bool isBypass = false;
	float speedMod = 1;

public:
	MovePlayerPacket* jetpack = nullptr;
	Jetpack();
	~Jetpack();

	virtual void onTick(C_GameMode* gm) override;
	// TODO: isFlashMode removed from IModule in 1.21.123
	//virtual bool isFlashMode() override;

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
