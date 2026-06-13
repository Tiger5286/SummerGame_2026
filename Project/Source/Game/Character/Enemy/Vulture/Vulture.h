#pragma once
#include "../EnemyBase.h"
#include "System/Animator.h"

class VultureStateBase;
class VultureStateIdle;
class VultureStateHit;

class Vulture : public EnemyBase, public std::enable_shared_from_this<Vulture>
{
public:
    Vulture() = default;
    ~Vulture() = default;

	void Init() override;
	void End() override;
	void Update() override;
	void Draw() override;

	void OnCollision(Character& other) override {}

	void OnHitAttack(int damage) override;

private:
	void CheckChangeState();

private:
	std::shared_ptr<VultureStateBase> m_pState = nullptr;
	Animator m_anim;
	float m_angle = 0.0f;
	float m_drawAngle = 0.0f;

	friend VultureStateIdle;
	friend VultureStateHit;
};

