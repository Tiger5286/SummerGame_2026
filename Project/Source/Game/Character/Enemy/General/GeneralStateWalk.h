#pragma once
#include "GeneralStateBase.h"
class GeneralStateWalk :
    public GeneralStateBase
{
public:
    GeneralStateWalk() = default;
    ~GeneralStateWalk() = default;

    void OnEnter() override;
    void Update() override;
    void Exit() override;
private:
};

