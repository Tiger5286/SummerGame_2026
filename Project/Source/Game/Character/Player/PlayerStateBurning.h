#pragma once
#include "PlayerStateBase.h"
class PlayerStateBurning :
    public PlayerStateBase
{
public:
    PlayerStateBurning() = default;
    ~PlayerStateBurning() = default;

    void Enter(std::weak_ptr<Player> pPlayer) override;
    void Update() override;
    void Exit() override;
private:

};

