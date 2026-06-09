#pragma once
#include "PlayerStateBase.h"

class Attack;
class BurningWing;

class PlayerStateBurning :
    public PlayerStateBase
{
public:
    PlayerStateBurning() = default;
    ~PlayerStateBurning() = default;

    void Enter(std::weak_ptr<Player> pPlayer) override;
    void Update() override;
    void Exit() override;
    void Draw() override;
private:
    int m_frame = 0;
    std::shared_ptr<Attack> m_pAtk = nullptr;
    std::shared_ptr<BurningWing> m_pWing = nullptr;
};