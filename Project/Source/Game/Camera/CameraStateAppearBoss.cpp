#include "CameraStateAppearBoss.h"
#include "Camera.h"

void CameraStateAppearBoss::Enter(std::weak_ptr<Camera> pCamera)
{
	m_pCamera = pCamera;
	auto camera = m_pCamera.lock();
	camera->m_pos = Vector3(3100, 130, -6300);
	camera->m_targetPos = Vector3(3100, 0, -6800);
}

void CameraStateAppearBoss::Update()
{
}

void CameraStateAppearBoss::Exit()
{
}
