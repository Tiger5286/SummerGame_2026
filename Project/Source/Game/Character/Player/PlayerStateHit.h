#pragma once
#include "PlayerStateBase.h"
class PlayerStateHit :
    public PlayerStateBase
{
public:
    PlayerStateHit() = default;
    ~PlayerStateHit() = default;

    void OnEnter() override;
    void Update() override;
    void Exit() override;

    HitAttribute GetHitAttribute() override { return HitAttribute::Invincible; }
};

