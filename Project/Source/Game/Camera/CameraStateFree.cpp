#include "CameraStateFree.h"
#include "Singleton/Input.h"
#include "Camera.h"
#include "Utility/Matrix4x4.h"
#include "Utility/MyLib.h"
#include "../Character/Player/Player.h"

namespace
{
	// カメラの回転速度
	constexpr float kRotSpeed = 0.05f;

	// カメラのX軸回転の上限と下限
	constexpr float kMaxAngleX = DX_PI_F / 4.0f - 0.1f;
	constexpr float kMinAngleX = -DX_PI_F / 4.0f + 0.1f;

	// 注視点との距離
	constexpr float kTargetDist = 300.0f;
	constexpr float kTargetDistBoss = 400.0f;

	// オフセット
	constexpr float kOffsetX = -75.0f;
	const Vector3 kTargetOffset = { 0.0f,100.0f,0.0f };
	const Vector3 kPosOffset = { 0.0f,200.0f,0.0f };

	// ターゲットがどれくらい注視方向からずれたら戻すかの角度
	constexpr float kLockonFixAngleY = DX_PI_F / 4.0f;
	constexpr float kLockonFixAngleX = DX_PI_F / 9.0f;
	// ターゲットにどれだけ近かったら注視方向の制限をしないか
	constexpr float kNotLockonDist = 200.0f;
	// ロックオンの最大補正速度
	constexpr float kMaxLockonSpeed = 0.05f;

	// ボス戦時のカメラの位置Yオフセット
	constexpr float kBossFightCameraOffsetY = 100.0f;
}

void CameraStateFree::Enter(std::weak_ptr<Camera> pCamera)
{
	m_pCamera = pCamera;
}

void CameraStateFree::Update()
{
	auto camera = m_pCamera.lock();
	auto player = camera->m_pPlayer.lock();

	// スティック入力に応じて角度を更新
	auto rightStick = Input::GetInstance().GetStickInput(MyLib::LR::Right);
	camera->m_angleY += rightStick.x * kRotSpeed;
	if (camera->m_angleY < 0.0f) camera->m_angleY += DX_TWO_PI_F;
	if (camera->m_angleY > DX_TWO_PI_F) camera->m_angleY -= DX_TWO_PI_F;
	camera->m_angleX -= rightStick.y * kRotSpeed;
	if (camera->m_angleX > kMaxAngleX) camera->m_angleX = kMaxAngleX;
	if (camera->m_angleX < kMinAngleX) camera->m_angleX = kMinAngleX;

	// ロックオンの処理
	if (camera->m_pTarget != nullptr)
	{
		// 距離が遠い場合のみロックオンする
		float playerToTargetSquaredDist = (camera->m_pTarget->GetPos() - player->GetPos()).SquaredLength();
		if (playerToTargetSquaredDist > kNotLockonDist * kNotLockonDist)
		{
			LockonUpdate();
		}
	}

	// プレイヤーの位置をもとにカメラの位置と注視点を設定
	// 位置を設定
	// デフォルトの向きのベクトルを生成
	Vector3 pos = MyLib::kDefaultDir;
	pos.Normalize();
	// ベクトルの長さを注視点との距離にする
	// ボス戦時はそれ専用の距離になる
	if (camera->m_isBossBattle)
	{
		pos *= kTargetDistBoss;
	}
	else
	{
		pos *= kTargetDist;
	}
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

	// 注視点とカメラの位置を右にずらす
	auto forwardVec = target - pos;
	auto right = forwardVec.Cross(Vector3::Up()).Normalized();
	pos += right * kOffsetX;
	target += right * kOffsetX;
	if (camera->m_isBossBattle)
	{
		pos.y += kBossFightCameraOffsetY;
		target.y += kBossFightCameraOffsetY;
	}

	camera->m_calcPos = pos;
	camera->m_calcTargetPos = target;
}

void CameraStateFree::Exit()
{

}

void CameraStateFree::LockonUpdate()
{
	auto camera = m_pCamera.lock();
	auto player = camera->m_pPlayer.lock();

	// プレイヤーから敵までのベクトル
	Vector3 toTarget = camera->m_pTarget->GetPos() - player->GetPos();

	// Y軸回転の補正
	{
		float targetAngleY = atan2f(toTarget.x, toTarget.z);	// ターゲットへの方向
		// 現在のカメラの角度からターゲットへの角度の差を計算
		float diffY = targetAngleY - camera->m_angleY;
		diffY = MyLib::WrapAngle(diffY);	// -π~+πの範囲に丸める

		if (abs(diffY) > kLockonFixAngleY)
		{
			// 角度の正負で1.0か-1.0の数値を生成
			float sign = 1.0f;
			if (diffY < 0.0f) sign = -1.0f;

			// 修正したい角度の量
			float fixAngle = abs(diffY) - kLockonFixAngleY;
			// 角度が大きすぎたら補正
			if (fixAngle > kMaxLockonSpeed) fixAngle = kMaxLockonSpeed;
			// 角度を修正
			camera->m_angleY += fixAngle * sign;
		}
	}
	// X軸回転の補正
	{
		Vector3 toTargetXZ = toTarget;
		toTargetXZ.y = 0.0f;
		// 水平距離
		float distXZ = toTargetXZ.Length();
		// 水平距離と高低差から理想のX軸角度を計算
		float targetAngleX = atan2f(-toTarget.y, distXZ);

		// 現在のX軸角度と理想のX軸角度の差
		float diffX = targetAngleX - camera->m_angleX;

		if (abs(diffX) > kLockonFixAngleX)
		{
			// 角度の正負で1.0か-1.0の数値を生成
			float sign = 1.0f;
			if (diffX < 0.0f) sign = -1.0f;

			// 修正したい角度の量
			float fixAngle = abs(diffX) - kLockonFixAngleX;
			// 角度が大きすぎたら補正
			if (fixAngle > kMaxLockonSpeed) fixAngle = kMaxLockonSpeed;
			// 角度を修正
			camera->m_angleX += fixAngle * sign;
		}
	}
}
