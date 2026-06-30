#pragma once
#include <memory>

class Camera;

class CameraSetter
{
public:
	static CameraSetter& GetInstance();
	~CameraSetter() = default;

	CameraSetter(const CameraSetter&) = delete;
	CameraSetter& operator=(const CameraSetter&) = delete;
private:
	CameraSetter() = default;
public:
	void Init(std::shared_ptr<Camera> pCamera);
	void SetCameraSetting();

private:
	std::shared_ptr<Camera> m_pCamera;
};

