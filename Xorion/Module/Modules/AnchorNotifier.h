#pragma once
#include "Module.h"

class AnchorNotifier : public IModule {
public:
    AnchorNotifier();
    ~AnchorNotifier();

    virtual const char* getModuleName() override;
    virtual void onTick(GameMode* gm) override;
};
