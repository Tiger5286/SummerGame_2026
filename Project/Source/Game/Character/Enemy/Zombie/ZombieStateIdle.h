#pragma once
#include "ZombieStateBase.h"
class ZombieStateIdle :
    public ZombieStateBase
{
	ZombieStateIdle() = default;
	virtual ~ZombieStateIdle() = default;

	void Enter(std::shared_ptr<Zombie> pZombie) override;
	void Update() override;
	void Exit() override;
};

