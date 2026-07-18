#pragma once
#include "GeneralStateBase.h"
class GeneralStateDeath :
    public GeneralStateBase
{
public:
    GeneralStateDeath() = default;
    ~GeneralStateDeath() = default;

    void OnEnter() override;
    void Update() override;
    void Exit() override;

private:

};

