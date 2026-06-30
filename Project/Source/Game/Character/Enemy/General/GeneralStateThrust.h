#pragma once
#include "GeneralStateBase.h"

class Attack;

class GeneralStateThrust :
    public GeneralStateBase
{
public:
    GeneralStateThrust() = default;
    ~GeneralStateThrust() = default;

    void Enter(std::weak_ptr<Character> pOwner) override;
    void Update() override;
    void Exit() override;
    void Draw() override;
private:
    int m_frame = 0;
    std::shared_ptr<Attack> m_pAtk = nullptr;
};

