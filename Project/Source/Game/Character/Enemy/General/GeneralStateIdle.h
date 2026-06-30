#pragma once
#include "GeneralStateBase.h"
class GeneralStateIdle :
    public GeneralStateBase
{
public:
    GeneralStateIdle() = default;
    ~GeneralStateIdle() = default;

    void Enter(std::weak_ptr<Character> pOwner) override;
    void Update() override;
    void Exit() override;
private:

};

