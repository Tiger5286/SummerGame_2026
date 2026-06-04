#pragma once
#include "PlayerStateBase.h"

class Attack;

class PlayerStateAttack :
    public PlayerStateBase
{
public:
    PlayerStateAttack() = default;
    ~PlayerStateAttack() = default;

    void Enter(std::weak_ptr<Player> pPlayer) override;
    void Update() override;
    void Exit() override;
    void Draw() override;

private:
    int m_comboIndex = 0;
    bool m_isCanTransNextCombo = false;
    bool m_isOnCollider = false;
    bool m_isOffCollider = false;

    std::shared_ptr<Attack> m_pAtk = nullptr;
};

