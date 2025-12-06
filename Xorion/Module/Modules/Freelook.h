#pragma once

#include "Module.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/Vec2.h"

class Freelook : public IModule {
public:
	// Member variables needed by Freelook.cpp implementation
	int resetViewTick = -1;
	bool redirectMouse = false;
	vec2_t initialViewAngles = {0, 0};
	vec2_t lastCameraAngle = {0, 0};
	vec2_t oldPos = {0, 0};
	bool isThirdPerson = false;
	bool cameraFacesFront = false;

	Freelook();
	~Freelook();

	virtual const char* getModuleName() override;
	void onEnable() override;
	void onDisable() override;
	void onPostRender(C_MinecraftUIRenderContext* mode) override;
	void onTick(C_GameMode* mode) override;
};