#pragma once
#include "ZombieStateBase.h"

class Attack;

class ZombieStateAttack :
    public ZombieStateBase
{
public:
	ZombieStateAttack() = default;
	virtual ~ZombieStateAttack() = default;

	void Enter(std::weak_ptr<Zombie> pZombie) override;
	void Update() override;
	void Exit() override;
	void Draw() override;
private:
	bool m_isOnCollider = false;
	bool m_isOffCollider = false;

	std::shared_ptr<Attack> m_pAtk = nullptr;
};

