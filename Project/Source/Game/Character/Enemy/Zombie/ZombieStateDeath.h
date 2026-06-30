#pragma once
#include "ZombieStateBase.h"
class ZombieStateDeath :
    public ZombieStateBase
{
public:
	ZombieStateDeath() = default;
	virtual ~ZombieStateDeath() = default;

	void Enter(std::weak_ptr<Character> pOwner) override;
	void Update() override;
	void Exit() override;
};

