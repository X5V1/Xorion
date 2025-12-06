#pragma once

#include "Module.h"

class ClickGuiMod : public IModule {
public:
	ClickGuiMod();
	~ClickGuiMod();

	bool showTooltips = true;

	// Inherited via IModule
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx) override;
	virtual const char* getModuleName() override;
	virtual void onDisable() override;
	virtual void onEnable() override;
	virtual bool allowAutoStart() override;
	// TODO: onLoadConfig/onSaveConfig removed from IModule in 1.21.123
	//virtual void onLoadConfig(void* conf) override;
	//virtual void onSaveConfig(void* conf) override;
};
