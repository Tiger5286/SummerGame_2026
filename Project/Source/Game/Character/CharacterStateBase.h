#pragma once
#include <memory>

class Character;

class CharacterStateBase : public std::enable_shared_from_this<CharacterStateBase>
{
public:
	CharacterStateBase() = default;
	virtual ~CharacterStateBase() = default;

	virtual void Enter(std::weak_ptr<Character> pOwner) = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;
	virtual void Draw() {}

	std::shared_ptr<CharacterStateBase> GetNextState()
	{
		return m_pNextState;
	}

	void ChangeState(std::shared_ptr<CharacterStateBase> pNextState)
	{
		m_pNextState = pNextState;
	}

protected:
	std::weak_ptr<Character> m_pOwner;
	std::shared_ptr<CharacterStateBase> m_pNextState = nullptr;
};
