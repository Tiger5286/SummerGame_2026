#pragma once
#include "PlayerStateBase.h"
class PlayerStateFall :
    public PlayerStateBase
{
public:
    PlayerStateFall() = default;
    ~PlayerStateFall() = default;

    void OnEnter() override;
    void Update() override;
    void Exit() override;
};

