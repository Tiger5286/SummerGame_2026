#pragma once
#include <memory>

class Player;

class PlayerStateBase
{
public:
	PlayerStateBase() = default;
	virtual ~PlayerStateBase() = default;

	virtual void Enter(std::weak_ptr<Player> pPlayer) abstract;
	virtual void Update() abstract;
	virtual void Exit() abstract;

	std::shared_ptr<PlayerStateBase> GetNextState()
	{
		return m_pNextState;
	}

	void ChangeState(std::shared_ptr<PlayerStateBase> pNextState)
	{
		m_pNextState = pNextState;
	}

protected:
	std::weak_ptr<Player> m_pPlayer;
	std::shared_ptr<PlayerStateBase> m_pNextState = nullptr;
};

