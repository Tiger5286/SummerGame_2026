#pragma once
#include "ZombieStateBase.h"
class ZombieStateDeath :
    public ZombieStateBase
{
public:
	ZombieStateDeath() = default;
	virtual ~ZombieStateDeath() = default;

	void Enter(std::weak_ptr<Zombie> pZombie) override;
	void Update() override;
	void Exit() override;
};

