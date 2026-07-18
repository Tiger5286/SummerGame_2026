#pragma once
#include "ZombieStateBase.h"
class ZombieStateIdle :
    public ZombieStateBase
{
public:
	ZombieStateIdle() = default;
	virtual ~ZombieStateIdle() = default;

	void OnEnter() override;
	void Update() override;
	void Exit() override;
};

