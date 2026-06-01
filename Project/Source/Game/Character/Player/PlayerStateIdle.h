#pragma once
#include "PlayerStateBase.h"
class PlayerStateIdle :
    public PlayerStateBase
{
public:
    PlayerStateIdle() = default;
    ~PlayerStateIdle() = default;

    void Enter(std::weak_ptr<Player> pPlayer) override;
    void Update() override;
    void Exit() override;
};

