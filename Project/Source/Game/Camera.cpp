#include "Camera.h"
#include <cmath>
#include "EffekseerForDXLib.h"
#include "../Utility/Matrix4x4.h"

#include "../System/Input.h"
#include "../Game/GameObject/Player.h"

namespace
{
	// カメラの初期位置と初期注視点
	const Vector3 kFirstPos = { 0.0f,0.0f,-700.0f };
	const Vector3 kFirstTarget = { 0.0f,0.0f,0.0f };
	// 視野角
	constexpr float kFov = DX_PI_F / 3.0f;	// 60度
	// NearFar
	constexpr float kNear = 10.0f;
	constexpr float kFar = 5000.0f;

	// オフセット
	constexpr float kOffsetX = -75.0f;
	const Vector3 kTargetOffset = { 0.0f,100.0f,0.0f };
	const Vector3 kPosOffset = { 0.0f,200.0f,0.0f };

	// 注視点との距離
	constexpr float kTargetDis = 300.0f;

	// カメラの回転速度
	constexpr float kRotSpeed = 0.05f;

	// カメラのX軸回転の上限と下限
	constexpr float kMaxAngleX = DX_PI_F / 4.0f - 0.1f;
	constexpr float kMinAngleX = -DX_PI_F / 4.0f + 0.1f;

	// ターゲットがどれくらい注視方向からずれたら戻すかの角度
	constexpr float kLockonFixAngle = DX_PI_F / 6.0f;

	// Lerpの定数
	constexpr float kLerpT = 0.3f;
}

Camera::Camera(Input& input):
	m_input(input)
{
}

Camera::~Camera()
{
}

void Camera::Init()
{
	// カメラの初期設定
	m_pos = kFirstPos;
	m_targetPos = kFirstTarget;
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLib(), m_targetPos.ToDxLib());
	SetupCamera_Perspective(kFov);
	SetCameraNearFar(kNear, kFar);
	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();

	// 角度を補正
	if (m_angleY < 0.0f) m_angleY += DX_TWO_PI_F;
	if (m_angleY > DX_TWO_PI_F) m_angleY -= DX_TWO_PI_F;
}

void Camera::Update()
{
	// スティック入力に応じて角度を更新
	auto rightStick = m_input.GetStickInput(LR::Right);
	m_angleY += rightStick.x * kRotSpeed;
	if (m_angleY < 0.0f) m_angleY += DX_TWO_PI_F;
	if (m_angleY > DX_TWO_PI_F) m_angleY -= DX_TWO_PI_F;
	m_angleX -= rightStick.y * kRotSpeed;
	if (m_angleX > kMaxAngleX) m_angleX = kMaxAngleX;
	if (m_angleX < kMinAngleX) m_angleX = kMinAngleX;

	Vector3 pos;
	Vector3 target;
	float dif = 0.0f;
	do
	{
		// プレイヤーの位置をもとにカメラの位置と注視点を設定
		// 位置を設定
		// 適当なベクトルを生成
		pos = { 0,0,-1 };
		pos.Normalize();
		// ベクトルの長さを注視点との距離にする
		pos *= kTargetDis;
		// 変形用の行列を生成
		auto rotYMtx = Matrix4x4::GetRotY(m_angleY);
		auto rotXMtx = Matrix4x4::GetRotX(m_angleX);
		auto transMtx = Matrix4x4::GetTranslate(m_playerPos + kPosOffset);
		// 行列を合成
		auto mtx = rotXMtx * rotYMtx * transMtx;
		// ベクトルを変形
		pos = mtx * pos;

		// 注視点を設定
		target = m_playerPos + kTargetOffset;
		// ロックオンターゲット
		// カメラの向き、プレイヤー→ターゲットの二つのベクトルを生成
		Vector3 cameraDir = (m_targetPos - m_pos);
		cameraDir.y = 0.0f;		// カメラの水平方向のみを見る
		cameraDir.Normalize();
		Vector3 playerToTargetVec = m_pTarget->GetPos() - m_playerPos;
		// 二つのベクトルの角度の差を求める			// cameraDirは正規化されているので1.0
		float dif = cameraDir.Dot(playerToTargetVec) / (1.0f * playerToTargetVec.Length());
		if (m_pTarget != nullptr)
		{
			// ターゲットが視界から外れようとしたら
			if (dif < cosf(kLockonFixAngle))
			{
				// 外積でどっちに向きを修正すべきか判定
				auto cross = cameraDir.Cross(playerToTargetVec);
				if (cross.y > 0)
				{
					m_angleY += /*abs(rightStick.x) **/ kRotSpeed;
				}
				else
				{
					m_angleY -= /*abs(rightStick.x) **/ kRotSpeed;
				}
			}
		}
		// difがいいかんじの値になるまでdowhileで回そうとしたけどなんかうまくいかない
		dif = cameraDir.Dot(playerToTargetVec) / (1.0f * playerToTargetVec.Length());
		float temp = cosf(kLockonFixAngle);
	} while (false);

	// 注視点とカメラの位置を右にずらす
	auto forwardVec = target - pos;
	auto right = forwardVec.Cross(Vector3::Up()).Normalized();
	pos += right * kOffsetX;
	target += right * kOffsetX;

	// マップとの当たり判定
	auto result = MV1CollCheck_Line(m_mapHandle, -1, (m_playerPos + kPosOffset).ToDxLib(), pos.ToDxLib());
	if (result.HitFlag)
	{
		pos = Vector3::FromDxLib(result.HitPosition);
	}

	// 注視点を滑らかに移動させる
	m_targetPos.Lerp(target, kLerpT);
	// カメラの位置を滑らかに移動させる
	m_pos.Lerp(pos, kLerpT);

	// 位置と注視点を反映
	SetCameraPositionAndTarget_UpVecY(m_pos.ToDxLib(), m_targetPos.ToDxLib());
	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();
}