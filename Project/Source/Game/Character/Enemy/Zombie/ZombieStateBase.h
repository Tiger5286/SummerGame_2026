#pragma once
#include "../../CharacterStateBase.h"
#include <memory>

class Zombie;

class ZombieStateBase : public CharacterStateBase
{
public:
	ZombieStateBase() = default;
	virtual ~ZombieStateBase() = default;

	virtual void Enter(std::weak_ptr<Character> pOwner) override = 0;
	virtual void Update() override = 0;
	virtual void Exit() override = 0;
	virtual void Draw() override {}

protected:
	std::weak_ptr<Zombie> m_pZombie;
};

