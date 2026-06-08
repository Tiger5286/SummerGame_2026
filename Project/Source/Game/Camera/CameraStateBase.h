#pragma once
#include <memory>

class Camera;

class CameraStateBase
{
public:
	CameraStateBase() = default;
	virtual ~CameraStateBase() = default;

	virtual void Enter(std::weak_ptr<Camera> pCamera) = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;

	std::shared_ptr<CameraStateBase> GetNextState()
	{
		return m_pNextState;
	}

	void ChangeState(std::shared_ptr<CameraStateBase> pNextState)
	{
		m_pNextState = pNextState;
	}

protected:
	std::weak_ptr<Camera> m_pCamera;
	std::shared_ptr<CameraStateBase> m_pNextState = nullptr;
};

