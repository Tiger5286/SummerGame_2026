#pragma once
#include "GeneralStateBase.h"
class GeneralStateDeath :
    public GeneralStateBase
{
public:
    GeneralStateDeath() = default;
    ~GeneralStateDeath() = default;

    void Enter(std::weak_ptr<Character> pOwner) override;
    void Update() override;
    void Exit() override;

private:

};

