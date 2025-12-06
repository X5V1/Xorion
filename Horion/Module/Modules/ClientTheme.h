#pragma once
#include "Module.h"

class ClientTheme : public IModule {
public:
	int themeMode = 0; // 0 = Dark, 1 = Legacy
	ClientTheme();
	~ClientTheme();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	void onRender(MinecraftUIRenderContext* renderCtx);
};