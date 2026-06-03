#pragma once
#include "PlayerStateBase.h"

class PlayerAttackCollider;

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
    void Tracking();

private:
    int m_comboIndex = 0;
    bool m_isCanTransNextCombo = false;
    bool m_isOnCollider = false;
    bool m_isOffCollider = false;

    std::shared_ptr<PlayerAttackCollider> m_pAtkCol = nullptr;
};

