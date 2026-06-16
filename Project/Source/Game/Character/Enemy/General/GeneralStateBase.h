#pragma once
#include <memory>

class General;

class GeneralStateBase
{
public:
	GeneralStateBase() = default;
	virtual ~GeneralStateBase() = default;

	virtual void Enter(std::weak_ptr<General> pGeneral) = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;
	virtual void Draw() {}

	std::shared_ptr<GeneralStateBase> GetNextState()
	{
		return m_pNextState;
	}

	void ChangeState(std::shared_ptr<GeneralStateBase> pNextState)
	{
		m_pNextState = pNextState;
	}

protected:
	std::weak_ptr<General> m_pGeneral;
	std::shared_ptr<GeneralStateBase> m_pNextState = nullptr;
};

