#include "CameraStateFree.h"
#include "Singleton/Input.h"
#include "Camera.h"
#include "Utility/Matrix4x4.h"
#include "../Character/Player/Player.h"

namespace
{
	// カメラの回転速度
	constexpr float kRotSpeed = 0.05f;

	// カメラのX軸回転の上限と下限
	constexpr float kMaxAngleX = DX_PI_F / 4.0f - 0.1f;
	constexpr float kMinAngleX = -DX_PI_F / 4.0f + 0.1f;

	// 注視点との距離
	constexpr float kTargetDis = 300.0f;

	// オフセット
	constexpr float kOffsetX = -75.0f;
	const Vector3 kTargetOffset = { 0.0f,100.0f,0.0f };
	const Vector3 kPosOffset = { 0.0f,200.0f,0.0f };

	// ターゲットがどれくらい注視方向からずれたら戻すかの角度
	constexpr float kLockonFixAngle = DX_PI_F / 6.0f;
	// ターゲットにどれだけ近かったら注視方向の制限をしないか
	constexpr float kNotLockonDist = 200.0f;
	// ロックオンの最大補正速度
	constexpr float kMaxLockonSpeed = 0.3f;
}

void CameraStateFree::Enter(std::weak_ptr<Camera> pCamera)
{
	m_pCamera = pCamera;
}

void CameraStateFree::Update()
{
	auto camera = m_pCamera.lock();

	// スティック入力に応じて角度を更新
	auto rightStick = Input::GetInstance().GetStickInput(LR::Right);
	camera->m_angleY += rightStick.x * kRotSpeed;
	if (camera->m_angleY < 0.0f) camera->m_angleY += DX_TWO_PI_F;
	if (camera->m_angleY > DX_TWO_PI_F) camera->m_angleY -= DX_TWO_PI_F;
	camera->m_angleX -= rightStick.y * kRotSpeed;
	if (camera->m_angleX > kMaxAngleX) camera->m_angleX = kMaxAngleX;
	if (camera->m_angleX < kMinAngleX) camera->m_angleX = kMinAngleX;

	// プレイヤーの位置をもとにカメラの位置と注視点を設定
	auto player = camera->m_pPlayer.lock();
	// 位置を設定
	// 適当なベクトルを生成
	Vector3 pos = { 0,0,-1 };
	pos.Normalize();
	// ベクトルの長さを注視点との距離にする
	pos *= kTargetDis;
	// 変形用の行列を生成
	auto rotYMtx = Matrix4x4::GetRotY(camera->m_angleY);
	auto rotXMtx = Matrix4x4::GetRotX(camera->m_angleX);
	auto transMtx = Matrix4x4::GetTranslate(player->GetPos() + kPosOffset);
	// 行列を合成
	auto mtx = rotXMtx * rotYMtx * transMtx;
	// ベクトルを変形
	pos = mtx * pos;

	// 注視点を設定
	Vector3 target = player->GetPos() + kTargetOffset;
	// ロックオンターゲット
	if (camera->m_pTarget != nullptr)
	{
		// カメラのY軸の向きの補正
		// カメラの向き、プレイヤー→ターゲットの二つのベクトルを生成
		Vector3 cameraDir = (camera->m_targetPos - camera->m_pos);
		cameraDir.y = 0.0f;		// カメラの水平方向のみを見る
		cameraDir.Normalize();
		Vector3 playerToTargetVec = camera->m_pTarget->GetPos() - player->GetPos();
		float playerToTargetDist = playerToTargetVec.SquaredLength();
		playerToTargetVec.y = 0.0f;	// 水平方向のみを見る
		// 二つのベクトルの角度の差を求める			// cameraDirは正規化されているので1.0
		float dif = cameraDir.Dot(playerToTargetVec) / (1.0f * playerToTargetVec.Length());
		// ターゲットが視界から外れようとしたら、かつ一定の距離が開いていたら
		if (dif < cosf(kLockonFixAngle) && playerToTargetDist > kNotLockonDist * kNotLockonDist)
		{
			// 外積でどっちに向きを修正すべきか判定
			auto cross = cameraDir.Cross(playerToTargetVec);
			if (cross.y > 0)
			{
				float fixValue = cosf(kLockonFixAngle) - dif;
				if (fixValue > kMaxLockonSpeed)
				{
					fixValue = kMaxLockonSpeed;
				}
				camera->m_angleY += fixValue;
			}
			else
			{
				float fixValue = cosf(kLockonFixAngle) - dif;
				if (fixValue > kMaxLockonSpeed)
				{
					fixValue = kMaxLockonSpeed;
				}
				camera->m_angleY -= fixValue;
			}
		}
	}

	// 注視点とカメラの位置を右にずらす
	auto forwardVec = target - pos;
	auto right = forwardVec.Cross(Vector3::Up()).Normalized();
	pos += right * kOffsetX;
	target += right * kOffsetX;

	camera->m_calcPos = pos;
	camera->m_calcTargetPos = target;
}

void CameraStateFree::Exit()
{

}
