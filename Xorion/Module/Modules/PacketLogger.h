#pragma once

#include "Module.h"

class PacketLogger : public IModule {
public:
	// TODO: SettingEnum removed, settings system redesigned
	// SettingEnum packetType;
	// SettingEnum addressType;
	bool packetadd;
	bool authchunk = false;
	bool setTitle = false;
	PacketLogger();
	~PacketLogger();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onSendPacket(Packet* packet) override;
	// onSendClientPacket not in IModule base class
	// virtual void onSendClientPacket(Packet* packet) override;
};