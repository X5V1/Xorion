#pragma once
#include "Module.h"

class MaceHit : public IModule {
public:
    MaceHit();
    ~MaceHit();

    virtual const char* getModuleName() override;
    // onAttack not in IModule base class - commenting out
    // virtual void onAttack(Entity* attackedEntity) override;
};

