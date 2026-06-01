#pragma once
#include "../Character.h"
#include <memory>

class SphereCollider;

class PlayerAttackCollider : public Character
{
public:
	PlayerAttackCollider() = default;
	virtual ~PlayerAttackCollider() = default;

	void Init() override;
	void End() override;
	void Update() override;
	void Draw() override;



private:
	std::shared_ptr<SphereCollider> m_pCollider;
};

