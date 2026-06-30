#pragma once
#include "ZombieStateBase.h"
class ZombieStateHit :
    public ZombieStateBase
{
public:
	ZombieStateHit() = default;
	virtual ~ZombieStateHit() = default;

	void Enter(std::weak_ptr<Character> pOwner) override;
	void Update() override;
	void Exit() override;
};