#pragma once
#include "PlayerStateBase.h"
class PlayerStateDodge :
    public PlayerStateBase
{
public:
    PlayerStateDodge() = default;
    ~PlayerStateDodge() = default;

    void OnEnter() override;
    void Update() override;
    void Exit() override;

    HitAttribute GetHitAttribute() override { return HitAttribute::PerfectInvincible; }
private:
    int m_dodgeFrame = 0;
};

