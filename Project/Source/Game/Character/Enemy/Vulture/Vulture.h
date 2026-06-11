#pragma once
#include "../EnemyBase.h"
#include "System/Animator.h"

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
	Animator m_anim;
	float m_angle = 0.0f;
	float m_drawAngle = 0.0f;
};

