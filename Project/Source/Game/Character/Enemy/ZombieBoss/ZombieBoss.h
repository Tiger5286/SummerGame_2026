#pragma once
#include "../EnemyBase.h"
#include "System/Animator.h"

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

    int GetMaxHP() const;

private:
    Animator m_anim;
};