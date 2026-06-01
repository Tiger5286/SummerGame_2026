#pragma once
#include "ZombieStateBase.h"
class ZombieStateIdle :
    public ZombieStateBase
{
public:
	ZombieStateIdle() = default;
	virtual ~ZombieStateIdle() = default;

	void Enter(std::weak_ptr<Zombie> pZombie) override;
	void Update() override;
	void Exit() override;
};

