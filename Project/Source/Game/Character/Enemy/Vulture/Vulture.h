#pragma once
#include "../EnemyBase.h"
#include "System/Animator.h"

class VultureStateBase;
class VultureStateIdle;
class VultureStateHit;
class VultureStateDeath;
class VultureStateDown;
class VultureStateAttack;

class Vulture : public EnemyBase//, public std::enable_shared_from_this<Vulture>
{
public:
    Vulture() = default;
    ~Vulture() = default;

	void Init() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void OnCollision(Character& other) override {}

	void OnHitAttack(const MyLib::AttackData& atkData) override;

	const int kMaxHp = 140;
	int GetMaxHP() const override { return kMaxHp; }

private:
	void KeepHeight();

private:
	Animator m_anim;

	bool m_isFlying = true;
	float m_flyHeight = 300.0f;

	friend VultureStateIdle;
	friend VultureStateHit;
	friend VultureStateDeath;
	friend VultureStateDown;
	friend VultureStateAttack;
};

