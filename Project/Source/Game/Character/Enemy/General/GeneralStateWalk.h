#pragma once
#include "GeneralStateBase.h"
class GeneralStateWalk :
    public GeneralStateBase
{
public:
    GeneralStateWalk() = default;
    ~GeneralStateWalk() = default;

    void Enter(std::weak_ptr<Character> pOwner) override;
    void Update() override;
    void Exit() override;
private:
};

