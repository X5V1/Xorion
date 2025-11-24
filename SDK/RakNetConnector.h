#pragma once

#include <string>
#include "TextHolder.h"

// Use struct consistently to avoid C4099 warnings.
struct RakNetConnector {
private:
    char pad_0x0[0x420];  // 0x0

public:
    TextHolder numericalIp;  // 0x420
    TextHolder serverIp;     // 0x440 (implied offset after TextHolder)
    uint32_t   serverPort;   // 0x460

private:
    virtual ~RakNetConnector();

public:
    virtual TextHolder* getLocalIps(TextHolder*);
    virtual TextHolder* getLocalIp(TextHolder*);
    virtual __int64     getPort(void);
    virtual __int64     getRefinedLocalIps(void);
    virtual int         getConnectedGameInfo(void);
    virtual void        setupNatPunch(bool);
    virtual int         getNatPunchInfo(void);
    virtual void        startNatPunchingClient(std::string const&, short);
    virtual void        addConnectionStateListener(__int64*);
    virtual void        removeConnectionStateListener(__int64*);
    virtual bool        isIPv4Supported(void);
    virtual bool        isIPv6Supported(void);
    virtual int         getIPv4Port(void);
    virtual int         getIPv6Port(void);
    virtual void        host(__int64 const&);
    virtual void        connect(void*);
    virtual void        disconnect(void);
    virtual void        tick(void);
    virtual void        runEvents(void);
    virtual bool        isServer(void);
    virtual int         getTransportIdentifier(void);
    virtual int         getPeer(void);
    virtual int         getPeer1(void);
    virtual void        _onDisable(void);
    virtual void        _onEnable(void);

public:
    inline bool isOnAServer() const { return !(serverIp.getTextLength() < 1); }
};
