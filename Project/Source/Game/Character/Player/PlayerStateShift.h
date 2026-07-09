#pragma once
#include "PlayerStateBase.h"
#include "Utility/Vector3.h"

class PlayerStateShift :
    public PlayerStateBase
{
public:
    PlayerStateShift() = default;
    ~PlayerStateShift() = default;

    void Enter(std::weak_ptr<Character> pOwner) override;
    void Update() override;
    void Exit() override;
private:
    bool m_isPlayedShiftEffect = false;
    bool m_isPlayedShiftEndEffect = false;
    Vector3 m_playerToTarget;
    float m_moveDist = 1000.0f;
};

