#pragma once
#include "PlayerStateBase.h"
class PlayerStateFall :
    public PlayerStateBase
{
public:
    PlayerStateFall() = default;
    ~PlayerStateFall() = default;

    void Enter(std::shared_ptr<Player> pPlayer) override;
    void Update() override;
    void Exit() override;
};

