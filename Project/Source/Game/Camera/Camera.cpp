#include "Camera.h"
#include <cmath>
#include "EffekseerForDXLib.h"
#include "Utility/Matrix4x4.h"
#include "Singleton/Input.h"
#include "Game/Character/Player/Player.h"

#include "CameraStateFree.h"

namespace
{
	// カメラの初期位置と初期注視点
	const Vector3 kFirstPos = { 75.0f, 150.0f,-300.0f };
	const Vector3 kFirstTarget = { 75.0f,100.0f,0.0f };
	// 視野角
	constexpr float kFov = DX_PI_F / 3.0f;	// 60度
	// NearFar
	constexpr float kNear = 10.0f;
	constexpr float kFar = 5000.0f;

	// オフセット
	const Vector3 kPosOffset = { 0.0f,200.0f,0.0f };

	// Lerpの定数
	constexpr float kLerpT = 0.3f;
}

void Camera::Init()
{
	// カメラの初期設定
	m_calcPos = kFirstPos;
	m_pos = kFirstPos;
	m_calcTargetPos = kFirstTarget;
	m_targetPos = kFirstTarget;
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLib(), m_targetPos.ToDxLib());
	SetupCamera_Perspective(kFov);
	SetCameraNearFar(kNear, kFar);
	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();

	// 角度を補正
	if (m_angleY < 0.0f) m_angleY += DX_TWO_PI_F;
	if (m_angleY > DX_TWO_PI_F) m_angleY -= DX_TWO_PI_F;

	// ステートの初期化
	m_pState = std::make_shared<CameraStateFree>();
	m_pState->ChangeState(m_pState);
	m_pState->Enter(weak_from_this());
	CheckChangeState();
}

void Camera::Update()
{
	CheckChangeState();

	auto player = m_pPlayer.lock();

	m_pState->Update();

	// マップとの当たり判定
	auto result = MV1CollCheck_Line(m_mapHandle, -1, (player->GetPos() + kPosOffset).ToDxLib(), m_calcPos.ToDxLib());
	if (result.HitFlag)
	{
		m_calcPos = Vector3::FromDxLib(result.HitPosition);
	}

	// 注視点を滑らかに移動させる
	m_targetPos.Lerp(m_calcTargetPos, kLerpT);
	// カメラの位置を滑らかに移動させる
	m_pos.Lerp(m_calcPos, kLerpT);

	// 位置と注視点を反映
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLib(), m_targetPos.ToDxLib());
	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();
}

void Camera::ChangeState(std::shared_ptr<CameraStateBase> pNextState)
{
	m_pState->ChangeState(pNextState);
}

void Camera::CheckChangeState()
{
	auto nextState = m_pState->GetNextState();
	// 次のステートがある場合は切り替え
	if (m_pState != nextState)
	{
		m_pState->Exit();

		m_pState = nextState;

		m_pState->Enter(weak_from_this());

		m_pState->ChangeState(m_pState);
	}
}
