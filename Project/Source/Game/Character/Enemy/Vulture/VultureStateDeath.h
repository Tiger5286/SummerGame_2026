#pragma once
#include "VultureStateBase.h"
class VultureStateDeath :
    public VultureStateBase
{
public:
    VultureStateDeath() = default;
    ~VultureStateDeath() = default;

    void Enter(std::weak_ptr<Character> pOwner) override;
    void Update() override;
    void Exit() override;
private:

};

