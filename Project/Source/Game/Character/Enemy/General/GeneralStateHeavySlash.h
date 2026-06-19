#pragma once
#include "GeneralStateBase.h"
class GeneralStateHeavySlash :
    public GeneralStateBase
{
public:
    GeneralStateHeavySlash() = default;
    ~GeneralStateHeavySlash() = default;

    void Enter(std::weak_ptr<General> pGeneral) override;
    void Update() override;
    void Exit() override;
private:

};

