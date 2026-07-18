#pragma once
#include "GeneralStateBase.h"

class Attack;

class GeneralStateHeavySlash :
    public GeneralStateBase
{
public:
    GeneralStateHeavySlash() = default;
    ~GeneralStateHeavySlash() = default;

    void OnEnter() override;
    void Update() override;
    void Exit() override;
    void Draw() override;
private:
    int m_frame = 0;
    std::shared_ptr<Attack> m_pAttack = nullptr;
};

