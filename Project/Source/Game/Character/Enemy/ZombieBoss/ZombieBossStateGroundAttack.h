#pragma once
#include "ZombieBossStateBase.h"

class Attack;

class ZombieBossStateGroundAttack :
    public ZombieBossStateBase
{
public:
    ZombieBossStateGroundAttack() = default;
    ~ZombieBossStateGroundAttack() = default;

    void OnEnter() override;
    void Update() override;
    void Exit() override;
	void Draw() override;
private:
    int m_frame = 0;

    std::shared_ptr<Attack> m_pAtk = nullptr;
};

