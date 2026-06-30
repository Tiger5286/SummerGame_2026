#pragma once
#include <memory>
#include "../CharacterStateBase.h"

class Player;

class PlayerStateBase : public CharacterStateBase
{
public:
	PlayerStateBase() = default;
	virtual ~PlayerStateBase() = default;

	virtual void Enter(std::weak_ptr<Character> pOwner) override = 0;
	virtual void Update() override = 0;
	virtual void Exit() override = 0;
	virtual void Draw() override {}

protected:
	std::weak_ptr<Player> m_pPlayer;
	std::shared_ptr<PlayerStateBase> m_pNextStatePlayer = nullptr;
};