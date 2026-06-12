#pragma once
#include <memory>

class Vulture;

class VultureStateBase
{
public:
	VultureStateBase() = default;
	virtual ~VultureStateBase() = default;

	virtual void Enter(std::weak_ptr<Vulture> pVulture) = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;
	virtual void Draw() {}

	std::shared_ptr<VultureStateBase> GetNextState()
	{
		return m_pNextState;
	}

	void ChangeState(std::shared_ptr<VultureStateBase> pNextState)
	{
		m_pNextState = pNextState;
	}

protected:
	std::weak_ptr<Vulture> m_pVulture;
	std::shared_ptr<VultureStateBase> m_pNextState = nullptr;
};

