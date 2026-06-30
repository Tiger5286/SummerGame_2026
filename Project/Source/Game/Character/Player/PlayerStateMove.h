#pragma once
#include "PlayerStateBase.h"
class PlayerStateMove :
    public PlayerStateBase
{
public:
    PlayerStateMove() = default;
    ~PlayerStateMove() = default;

    void Enter(std::weak_ptr<Character> pOwner) override;
    void Update() override;
    void Exit() override;
};

