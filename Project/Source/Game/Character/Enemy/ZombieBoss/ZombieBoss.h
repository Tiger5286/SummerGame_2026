#pragma once
#include "../EnemyBase.h"
#include "System/Animator.h"

class BossHpBar;

class ZombieBoss :
    public EnemyBase
{
public:
    ZombieBoss() = default;
    ~ZombieBoss() override = default;

    void Init() override;
    void End() override;
    void OnUpdate() override;
    void Draw() override;

	void OnHitAttack(const MyLib::AttackData& atkData) override;

    int GetMaxHP() const;

private:
    Animator m_anim;

	std::shared_ptr<BossHpBar> m_pBossBar = nullptr;

    friend class ZombieBossStateWalk;
    friend class ZombieBossStateIdle;
    friend class ZombieBossStateAttack;
    friend class ZombieBossStateGroundAttack;
    friend class ZombieBossStateDeath;
    friend class ZombieBossStateAppear;
};