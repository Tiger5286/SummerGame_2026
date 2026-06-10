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

    void Enter(std::weak_ptr<Player> pPlayer) override;
    void Update() override;
    void Exit() override;
    void Draw() override;
private:
    int m_frame = 0;
    float m_angle = 0.0f;
	float m_angleVel = 0.0f;
    std::shared_ptr<SpinWing> m_pWing = nullptr;
    std::shared_ptr<Attack> m_pAttackL = nullptr;
    std::shared_ptr<Attack> m_pAttackR = nullptr;
};

