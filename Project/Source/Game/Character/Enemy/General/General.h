#pragma once
#include "../EnemyBase.h"
#include "System/Animator.h"

class BossHpBar;

class GeneralStateBase;
class GeneralStateIdle;
class GeneralStateWalk;
class GeneralStateHeavySlash;
class GeneralStateThrust;
class GeneralStateProjectile;
class GeneralStateDeath;
class GeneralStateAppear;

class General : // このGeneralは将軍という意味のGeneral
    public EnemyBase
{
public:
    General() = default;
    ~General();

    void Init() override;
    void End() override;
    void OnUpdate() override;
    void Draw() override;

    void OnHitAttack(const MyLib::AttackData& atkData) override;

    const int kMaxHp = 3000;
    int GetMaxHP() const override { return kMaxHp; }

private:
    void AttackRandom();

private:
    int m_swordModelHandle = -1;
    Animator m_anim;

    const int kAttackCooltime = 180;
    int m_attackCooltime = kAttackCooltime;

    std::shared_ptr<BossHpBar> m_pBossBar = nullptr;

private:
    friend GeneralStateIdle;
    friend GeneralStateWalk;
    friend GeneralStateHeavySlash;
    friend GeneralStateThrust;
    friend GeneralStateProjectile;
    friend GeneralStateDeath;
    friend GeneralStateAppear;
};