#include "CameraStateAppearBoss.h"
#include "Camera.h"

#include "CameraStateFree.h"

namespace
{
	const Vector3 kPos = Vector3(3100, 130, -6300);
	const Vector3 kTargetPos = Vector3(3100, 0, -6800);

	constexpr int kStartShakeFrame = 100;
	constexpr int kShakeFrame = 120;
	constexpr float kShakePower = 5.0f;

	constexpr int kStateEndFrame = 270;
}

void CameraStateAppearBoss::Enter(std::weak_ptr<Camera> pCamera)
{
	m_pCamera = pCamera;
	auto camera = m_pCamera.lock();
	camera->m_pos = kPos;
	camera->m_targetPos = kTargetPos;
	camera->m_calcPos = kPos;
	camera->m_calcTargetPos = kTargetPos;
}

void CameraStateAppearBoss::Update()
{
	m_frame++;
	auto camera = m_pCamera.lock();

	// カメラを揺らす
	if (m_frame == kStartShakeFrame)
	{
		camera->SetShake(kShakeFrame, kShakePower);
	}

	// 一定時間たったら戻す
	if (m_frame > kStateEndFrame)
	{
		ChangeState(std::make_shared<CameraStateFree>());
		return;
	}
}

void CameraStateAppearBoss::Exit()
{
}
