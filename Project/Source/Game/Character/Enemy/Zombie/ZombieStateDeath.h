#pragma once
#include "ZombieStateBase.h"
class ZombieStateDeath :
    public ZombieStateBase
{
public:
	ZombieStateDeath() = default;
	virtual ~ZombieStateDeath() = default;

	void OnEnter() override;
	void Update() override;
	void Exit() override;
};

