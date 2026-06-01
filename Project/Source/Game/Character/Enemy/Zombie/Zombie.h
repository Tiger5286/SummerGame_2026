#pragma once
#include "../EnemyBase.h"
#include "System/Animator.h"

class Player;

class ZombieStateBase;
class ZombieStateIdle;
class ZombieStateMove;
class ZombieStateAttack;
class ZombieStateHit;

class Zombie : public EnemyBase
{
public:
	Zombie();
	virtual ~Zombie() override;

	void Init() override;
	void End() override;
	void Update() override;
	void Draw() override;

private:
	Animator m_anim;
	float m_angle = 0.0f;

	std::shared_ptr<ZombieStateBase> m_pState = nullptr;

	friend ZombieStateBase;
	friend ZombieStateIdle;
	friend ZombieStateMove;
	friend ZombieStateAttack;
	friend ZombieStateHit;
};

