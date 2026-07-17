#pragma once
#include "PlayerStateBase.h"
class PlayerStateDodge :
    public PlayerStateBase
{
public:
    PlayerStateDodge() = default;
    ~PlayerStateDodge() = default;

    void Enter(std::weak_ptr<Character> pOwner) override;
    void Update() override;
    void Exit() override;

    HitAttribute GetHitAttribute() override { return HitAttribute::PerfectInvincible; }
private:
    int m_dodgeFrame = 0;
};

