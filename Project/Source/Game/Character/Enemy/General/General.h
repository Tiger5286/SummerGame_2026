#pragma once
#include "../EnemyBase.h"
#include "System/Animator.h"

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
    int m_swordModelHandle = -1;
    Animator m_anim;
};