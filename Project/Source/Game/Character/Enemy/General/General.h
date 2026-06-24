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
    public EnemyBase,public std::enable_shared_from_this<General>
{
public:
    General() = default;
    ~General();

    void Init() override;
    void End() override;
    void Update() override;
    void Draw() override;

private:
    void CheckChangeState();

    void AttackRandom();

private:
    int m_swordModelHandle = -1;
    Animator m_anim;

    std::shared_ptr<GeneralStateBase> m_pState = nullptr;

    const int kAttackCooltime = 180;
    int m_attackCooltime = kAttackCooltime;

private:
    friend GeneralStateIdle;
    friend GeneralStateWalk;
    friend GeneralStateHeavySlash;
    friend GeneralStateThrust;
    friend GeneralStateProjectile;
};