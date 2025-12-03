#pragma once
#include "Module.h"

class MaceHit : public IModule {
public:
    MaceHit();
    ~MaceHit();

    virtual const char* getModuleName() override;
    virtual void onAttack(C_Entity* attackedEntity) override;
};
