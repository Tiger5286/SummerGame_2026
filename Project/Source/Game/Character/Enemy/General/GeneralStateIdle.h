#pragma once
#include "GeneralStateBase.h"
class GeneralStateIdle :
    public GeneralStateBase
{
public:
    GeneralStateIdle() = default;
    ~GeneralStateIdle() = default;

    void OnEnter() override;
    void Update() override;
    void Exit() override;
private:

};

