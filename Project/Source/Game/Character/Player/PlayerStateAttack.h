#pragma once
#include "PlayerStateBase.h"

class PlayerStateAttack :
    public PlayerStateBase
{
public:
    PlayerStateAttack() = default;
    ~PlayerStateAttack() = default;

    void Enter(std::weak_ptr<Player> pPlayer) override;
    void Update() override;
    void Exit() override;

private:
    void Tracking();

private:
    int m_comboIndex = 0;
    bool m_isCanTransNextCombo = false;
};

