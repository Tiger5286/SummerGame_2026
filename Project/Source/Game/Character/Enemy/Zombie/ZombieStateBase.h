#pragma once
#include <memory>

class Zombie;

class ZombieStateBase
{
public:
	ZombieStateBase() = default;
	virtual ~ZombieStateBase() = default;

	virtual void Enter(std::weak_ptr<Zombie> pZombie) abstract;
	virtual void Update() abstract;
	virtual void Exit() abstract;

	std::shared_ptr<ZombieStateBase> GetNextState()
	{
		return m_pNextState;
	}

	void ChangeState(std::shared_ptr<ZombieStateBase> pNextState)
	{
		m_pNextState = pNextState;
	}

protected:
	std::weak_ptr<Zombie> m_pZombie;
	std::shared_ptr<ZombieStateBase> m_pNextState = nullptr;
};

