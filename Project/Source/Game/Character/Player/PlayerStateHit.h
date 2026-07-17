#pragma once
#include "PlayerStateBase.h"
class PlayerStateHit :
    public PlayerStateBase
{
public:
    PlayerStateHit() = default;
    ~PlayerStateHit() = default;

    void Enter(std::weak_ptr<Character> pOwner) override;
    void Update() override;
    void Exit() override;

    HitAttribute GetHitAttribute() override { return HitAttribute::Invincible; }
};

