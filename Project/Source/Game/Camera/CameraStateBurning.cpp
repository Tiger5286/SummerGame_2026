#include "CameraStateBurning.h"
#include "Camera.h"
#include "../Character/Player/Player.h"
#include "Utility/Matrix4x4.h"

namespace
{
	// カメラワークの切り替えのフレーム数
	constexpr int kChangeCameraWorkFrame = 120;		// 攻撃発動までのフレーム数
	constexpr int kChangeCameraWorkFrame2 = 300;	// 攻撃が終わるまでのフレーム数

	// カメラの位置のオフセット
	const Vector3 kInitialPosOffset = Vector3(-200, 100, 0);	// カメラの初期位置のオフセット
	const Vector3 kInitialTargetOffset = Vector3(0, 100, 0);		// カメラの初期注視点のオフセット
	constexpr float kInitialAngle = -1.5f;	// カメラの初期角度
	constexpr float kAngleSpeed = 0.01f;	// カメラの角度の回転速度

	const Vector3 kAttackPosOffset = Vector3(0, 250, 400);	// 攻撃中のカメラ位置のオフセット
	const Vector3 kAttackTargetOffset = Vector3(0, 200, 0);	// 攻撃中のカメラ注視点のオフセット

	const Vector3 kFinalPosOffset = Vector3(75, 170, -300);	// 攻撃後のカメラ位置のオフセット
	const Vector3 kFinalTargetOffset = Vector3(75, 100, 0);	// 攻撃後のカメラ注視点のオフセット
}

void CameraStateBurning::Enter(std::weak_ptr<Camera> pCamera)
{
	m_pCamera = pCamera;
	auto camera = m_pCamera.lock();
	auto player = camera->m_pPlayer.lock();
	// カメラの位置をプレイヤーの左に設定
	camera->m_pos = player->GetPos() + Matrix4x4::GetRotY(player->GetAngle()) * kInitialPosOffset;
	camera->m_calcPos = player->GetPos() + Matrix4x4::GetRotY(player->GetAngle()) * kInitialPosOffset;
	// 注視点をプレイヤーに設定
	camera->m_targetPos = player->GetPos() + Matrix4x4::GetRotY(player->GetAngle()) * kInitialTargetOffset;
	camera->m_calcTargetPos = player->GetPos() + Matrix4x4::GetRotY(player->GetAngle()) * kInitialTargetOffset;
	// カメラの角度を設定
	m_angle = kInitialAngle;
	camera->m_angleX = 0.0f;
}

void CameraStateBurning::Update()
{
	auto camera = m_pCamera.lock();
	auto player = camera->m_pPlayer.lock();

	m_frame++;
	// 攻撃が発動するまでのカメラワーク
	if (m_frame < kChangeCameraWorkFrame)
	{
		// カメラの位置を徐々に移動させる
		m_posOffset.x += 200.0f / kChangeCameraWorkFrame;
		m_posOffset.y += 150.0f / kChangeCameraWorkFrame;
		m_posOffset.z += 400.0f / kChangeCameraWorkFrame;
		// カメラの角度を徐々に回転させる
		m_angle += kAngleSpeed;
		camera->m_calcPos = player->GetPos() + Matrix4x4::GetRotY(player->GetAngle() + m_angle) * (kInitialPosOffset + m_posOffset);
	}
	else if (m_frame < kChangeCameraWorkFrame2)	// 攻撃が発動してから攻撃が終わるまでのカメラワーク
	{
		camera->m_calcPos = player->GetPos() + Matrix4x4::GetRotY(player->GetAngle()) * kAttackPosOffset;
		camera->m_calcTargetPos = player->GetPos() + Matrix4x4::GetRotY(player->GetAngle()) * kAttackTargetOffset;
	}
	else	// 攻撃が終わってからのカメラワーク
	{	// カメラの位置を徐々に移動させる
		Vector3 finalPos = player->GetPos() + kFinalPosOffset * Matrix4x4::GetRotY(camera->m_angleY);
		Vector3 posVec = finalPos - camera->m_pos;
		posVec = posVec.Normalized() * posVec.Length() / 40;
		camera->m_calcPos += posVec;
		// カメラの注視点を徐々に移動させる
		Vector3 finalTarget = player->GetPos() + kFinalTargetOffset * Matrix4x4::GetRotY(camera->m_angleY);
		Vector3 targetVec = finalTarget - camera->m_targetPos;
		targetVec = targetVec.Normalized() * targetVec.Length() / 40;
		camera->m_calcTargetPos += targetVec;
	}
}

void CameraStateBurning::Exit()
{
}
