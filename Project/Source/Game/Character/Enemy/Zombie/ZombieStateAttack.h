#pragma once
#include "ZombieStateBase.h"
class ZombieStateAttack :
    public ZombieStateBase
{
public:
	ZombieStateAttack() = default;
	virtual ~ZombieStateAttack() = default;

	void Enter(std::weak_ptr<Zombie> pZombie) override;
	void Update() override;
	void Exit() override;
};

