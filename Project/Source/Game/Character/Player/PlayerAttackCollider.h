#pragma once
#include "../Character.h"
#include <memory>
#include <list>

class Player;
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

	void OnCollision(Character& other) override;

private:
	
	std::list<int> m_hitIds;

	friend Player;
};

