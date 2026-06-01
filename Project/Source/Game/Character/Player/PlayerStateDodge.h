#pragma once
#include "PlayerStateBase.h"
class PlayerStateDodge :
    public PlayerStateBase
{
public:
    PlayerStateDodge() = default;
    ~PlayerStateDodge() = default;

    void Enter(std::weak_ptr<Player> pPlayer) override;
    void Update() override;
    void Exit() override;

private:
    int m_dodgeFrame = 0;
};

