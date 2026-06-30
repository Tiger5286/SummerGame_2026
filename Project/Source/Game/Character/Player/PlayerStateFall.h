#pragma once
#include "PlayerStateBase.h"
class PlayerStateFall :
    public PlayerStateBase
{
public:
    PlayerStateFall() = default;
    ~PlayerStateFall() = default;

    void Enter(std::weak_ptr<Character> pOwner) override;
    void Update() override;
    void Exit() override;
};

