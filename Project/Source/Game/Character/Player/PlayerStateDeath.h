#pragma once
#include "PlayerStateBase.h"
class PlayerStateDeath :
    public PlayerStateBase
{
public:
    PlayerStateDeath() = default;
    ~PlayerStateDeath() = default;

    void Enter(std::weak_ptr<Character> pOwner) override;
    void Update() override;
    void Exit() override;

private:

};

