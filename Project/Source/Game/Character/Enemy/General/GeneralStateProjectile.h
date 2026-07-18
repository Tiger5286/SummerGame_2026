#pragma once
#include "GeneralStateBase.h"

class Attack;

class GeneralStateProjectile :
    public GeneralStateBase
{
public:
    GeneralStateProjectile() = default;
    ~GeneralStateProjectile() = default;

    void OnEnter() override;
    void Update() override;
    void Exit() override;
    void Draw() override;
private:
    int m_frame = 0;
    std::shared_ptr<Attack> m_pAtk = nullptr;
};

