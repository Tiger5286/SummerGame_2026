#pragma once
#include "PlayerStateBase.h"
class PlayerStateShift :
    public PlayerStateBase
{
public:
    PlayerStateShift() = default;
    ~PlayerStateShift() = default;

    void Enter(std::weak_ptr<Player> pPlayer) override;
    void Update() override;
    void Exit() override;
private:
    bool m_isPlayedEffect = false;
};

