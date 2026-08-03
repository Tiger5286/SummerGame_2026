#pragma once
#include "ZombieBossStateBase.h"
class ZombieBossStateDeath :
    public ZombieBossStateBase
{
public:
    ZombieBossStateDeath() = default;
    ~ZombieBossStateDeath() = default;

    void OnEnter() override;
    void Update() override;
    void Exit() override;
};

