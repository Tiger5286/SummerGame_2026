#pragma once
#include "PlayerStateBase.h"
class PlayerStateDeath :
    public PlayerStateBase
{
public:
    PlayerStateDeath() = default;
    ~PlayerStateDeath() = default;

    void OnEnter() override;
    void Update() override;
    void Exit() override;

    HitAttribute GetHitAttribute() override { return HitAttribute::PerfectInvincible; }
private:

};

