#pragma once

#include "Module.h"
#include <string>

class Impersonator : public IModule {
private:
	std::string targetPlayerName;

public:
	Impersonator();
	~Impersonator();

	// Getters and setters for target player
	inline std::string& getTargetPlayerName() { return targetPlayerName; }
	inline void setTargetPlayerName(const std::string& name) { targetPlayerName = name; }
	inline bool hasTarget() const { return !targetPlayerName.empty(); }
	inline void clearTarget() { targetPlayerName.clear(); }

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onSendPacket(Packet* packet) override;
};
