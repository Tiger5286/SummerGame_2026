#pragma once
#include "PlayerStateBase.h"
class PlayerStateMove :
    public PlayerStateBase
{
public:
    PlayerStateMove() = default;
    ~PlayerStateMove() = default;

    void OnEnter() override;
    void Update() override;
    void Exit() override;
};

