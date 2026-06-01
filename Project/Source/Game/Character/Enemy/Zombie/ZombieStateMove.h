#pragma once
#include "ZombieStateBase.h"
class ZombieStateMove :
    public ZombieStateBase
{
public:
	ZombieStateMove() = default;
	virtual ~ZombieStateMove() = default;

	void Enter(std::weak_ptr<Zombie> pZombie) override;
	void Update() override;
	void Exit() override;
};