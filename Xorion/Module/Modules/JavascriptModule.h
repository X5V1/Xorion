#pragma once
#include "Module.h"
#include "../../Scripting/ScriptInstance.h"

class JsScriptModule;

class JavascriptModule : public IModule {
private:
	std::weak_ptr<JsScriptModule> backingScriptModule;

public:
	JavascriptModule();
	~JavascriptModule();

	void setBackingScript(std::shared_ptr<JsScriptModule>);
	std::weak_ptr<JsScriptModule> getBackingScriptModule();

	// Inherited via IModule
	// TODO: onSaveConfig removed from IModule in 1.21.124
	//virtual void onSaveConfig(void*) override{}; // delete
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	void onEnable() override;
	void onDisable() override;
	void onLevelRender() override;
	void onPreRender(MinecraftUIRenderContext* renderCtx) override;
};
