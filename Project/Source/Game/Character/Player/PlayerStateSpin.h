#pragma once
#include "PlayerStateBase.h"
class PlayerStateSpin :
    public PlayerStateBase
{
public:
    PlayerStateSpin() = default;
    ~PlayerStateSpin() = default;

    void Enter(std::weak_ptr<Player> pPlayer) override;
    void Update() override;
    void Exit() override;
};

