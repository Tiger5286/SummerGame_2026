#pragma once
#include "../Character.h"
#include <memory>

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

private:
	friend Player;
};

