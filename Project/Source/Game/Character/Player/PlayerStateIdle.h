#pragma once
#include "PlayerStateBase.h"
class PlayerStateIdle :
    public PlayerStateBase
{
public:
    PlayerStateIdle() = default;
    ~PlayerStateIdle() = default;

    void OnEnter() override;
    void Update() override;
    void Exit() override;
};

