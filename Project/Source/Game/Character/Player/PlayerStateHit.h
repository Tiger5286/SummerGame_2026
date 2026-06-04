#pragma once
#include "PlayerStateBase.h"
class PlayerStateHit :
    public PlayerStateBase
{
public:
    PlayerStateHit() = default;
    ~PlayerStateHit() = default;

    void Enter(std::weak_ptr<Player> pPlayer) override;
    void Update() override;
    void Exit() override;
};

