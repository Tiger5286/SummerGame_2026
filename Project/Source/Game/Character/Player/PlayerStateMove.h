#pragma once
#include "PlayerStateBase.h"
class PlayerStateMove :
    public PlayerStateBase
{
public:
    PlayerStateMove() = default;
    ~PlayerStateMove() = default;

    void Enter(std::shared_ptr<Player> pPlayer) override;
    void Update() override;
    void Exit() override;
};

