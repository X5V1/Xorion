#pragma once

#include "Module.h"

class TestModule : public IModule {
private:
	int delay = 0;

public:
	Vec3 savepos;
	float float1 = 0;
	int int1 = 0;
	bool bool1 = true;
	// TODO: SettingEnum removed in 1.21.130
	//SettingEnum enum1 = SettingEnum(this);

	TestModule();
	~TestModule();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	// TODO: isFlashMode removed from IModule in 1.21.130
	//virtual bool isFlashMode() override;
	virtual void onEnable() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx) override;
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx) override;
	virtual void onSendPacket(Packet* p) override;
	// TODO: onSendClientPacket not in IModule base class
	// virtual void onSendClientPacket(Packet* p) override;
	// TODO: onMove removed from IModule in 1.21.130
	//virtual void onMove(MoveInputHandler* hand) override;
	virtual void onDisable() override;
	// TODO: onKey not in IModule base class
	// virtual void onKey(int key, bool isDown, bool& cancel) override;
	void onLevelRender() override;
};