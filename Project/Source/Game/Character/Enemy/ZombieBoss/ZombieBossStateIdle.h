#pragma once
#include "ZombieBossStateBase.h"
class ZombieBossStateIdle :
    public ZombieBossStateBase
{
public:
    ZombieBossStateIdle() = default;
    ~ZombieBossStateIdle() = default;

    void OnEnter() override;
    void Update() override;
	void Exit() override;
};

