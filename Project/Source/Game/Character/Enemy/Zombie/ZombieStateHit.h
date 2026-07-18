#pragma once
#include "ZombieStateBase.h"
class ZombieStateHit :
    public ZombieStateBase
{
public:
	ZombieStateHit() = default;
	virtual ~ZombieStateHit() = default;

	void OnEnter() override;
	void Update() override;
	void Exit() override;
};