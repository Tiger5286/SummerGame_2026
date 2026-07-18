#pragma once
#include "VultureStateBase.h"
class VultureStateDeath :
    public VultureStateBase
{
public:
    VultureStateDeath() = default;
    ~VultureStateDeath() = default;

    void OnEnter() override;
    void Update() override;
    void Exit() override;
private:

};

