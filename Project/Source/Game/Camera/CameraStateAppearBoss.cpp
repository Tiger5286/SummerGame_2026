#include "CameraStateAppearBoss.h"
#include "Camera.h"

#include "CameraStateFree.h"

namespace
{
	constexpr int kStartShakeFrame = 100;
	constexpr int kShakeFrame = 120;
	constexpr float kShakePower = 5.0f;

	constexpr int kStateEndFrame = 270;
}

void CameraStateAppearBoss::Enter(std::weak_ptr<Camera> pCamera)
{
	m_pCamera = pCamera;
	auto camera = m_pCamera.lock();
	camera->m_pos = camera->m_bossDirectionPos;
	camera->m_targetPos = camera->m_bossDirectionTargetPos;
	camera->m_calcPos = camera->m_bossDirectionPos;
	camera->m_calcTargetPos = camera->m_bossDirectionTargetPos;
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
