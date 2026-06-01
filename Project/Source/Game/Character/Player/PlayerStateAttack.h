#pragma once
#include "PlayerStateBase.h"
#include <vector>

class PlayerStateAttack :
    public PlayerStateBase
{
public:
    PlayerStateAttack() = default;
    ~PlayerStateAttack() = default;

    void Enter(std::shared_ptr<Player> pPlayer) override;
    void Update() override;
    void Exit() override;

private:
    int m_comboIndex = 0;
    bool m_isCanTransNextCombo = false;
};

