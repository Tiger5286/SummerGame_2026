#pragma once
#include "EnemyBase.h"
#include "System/Animator.h"

class Player;

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
};

