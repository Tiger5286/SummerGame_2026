#pragma once
#include "PlayerStateBase.h"

class SpinWing;
class Attack;

class PlayerStateSpin :
    public PlayerStateBase
{
public:
    PlayerStateSpin() = default;
    ~PlayerStateSpin() = default;

    void Enter(std::weak_ptr<Character> pOwner) override;
    void Update() override;
    void Exit() override;
    void Draw() override;

    HitAttribute GetHitAttribute() override { return HitAttribute::IgnoreFalter; }
private:
    int m_frame = 0;
    int m_effHandle = -1;
    std::shared_ptr<SpinWing> m_pWing = nullptr;
    std::shared_ptr<Attack> m_pAttackL = nullptr;
    std::shared_ptr<Attack> m_pAttackR = nullptr;
};

