#pragma once
#include "../EnemyBase.h"
#include "System/Animator.h"

class GeneralStateBase;
class GeneralStateIdle;
class GeneralStateWalk;
class GeneralStateHeavySlash;
class GeneralStateThrust;
class GeneralStateProjectile;

class General :
    public EnemyBase
{
public:
    General() = default;
    ~General();

    void Init() override;
    void End() override;
    void Update() override;
    void Draw() override;

private:
    void AttackRandom();

private:
    int m_swordModelHandle = -1;
    Animator m_anim;

    const int kAttackCooltime = 180;
    int m_attackCooltime = kAttackCooltime;

private:
    friend GeneralStateIdle;
    friend GeneralStateWalk;
    friend GeneralStateHeavySlash;
    friend GeneralStateThrust;
    friend GeneralStateProjectile;
};