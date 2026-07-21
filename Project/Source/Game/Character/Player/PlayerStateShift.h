#pragma once
#include "PlayerStateBase.h"
#include "Utility/Vector3.h"

class PlayerStateShift :
    public PlayerStateBase
{
private:
    const float kMoveDist = 1000.0f;

public:
    PlayerStateShift() = default;
    ~PlayerStateShift() = default;

    void OnEnter() override;
    void Update() override;
    void Exit() override;
private:
    bool m_isPlayedShiftEffect = false;
    bool m_isPlayedShiftEndEffect = false;
    Vector3 m_playerToTarget;
    float m_moveDist = kMoveDist;
    bool m_isTransShiftAttack = false;
};

