#include "PlayerStateMove.h"
#include "Player.h"
#include "Singleton/Input.h"
#include "Utility/Matrix4x4.h"

#include "PlayerStateIdle.h"

namespace
{
	// アニメーション名
	const std::wstring kRunAnimName = L"Player|Run";

	// 加速度
	constexpr float kMoveAccel = 3.0f;
	// 最大移動速度
	constexpr float kMaxMoveSpeed = 7.0f;
}

void PlayerStateMove::Enter(std::shared_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_pPlayer->m_anim.ChangeAnim(kRunAnimName);
}

void PlayerStateMove::Update()
{
	// 入力を取得
	auto& input = Input::GetInstance();
	// スティック入力がなくなったらIdleにする
	if (input.GetStickInput(LR::Left).SquaredLength() <= 0.0f)
	{
		ChangeState(std::make_shared<PlayerStateIdle>());
		return;
	}
	// 入力方向を向く
	m_pPlayer->RotateInputDir();

	// スティック入力を取得
	auto stick = input.GetStickInput(LR::Left);
	// 移動ベクトルに入力を反映する
	Vector3 moveVec;
	moveVec += Vector3(stick.x, 0.0f, stick.y) * kMoveAccel;
	// カメラの向きに応じて移動ベクトルを回転させる
	moveVec *= Matrix4x4::GetRotY(m_pPlayer->m_cameraAngleY);
	// 水平移動速度が上限を超えていなければ移動量を足す
	auto velXZ = m_pPlayer->m_vel;
	velXZ.y = 0.0f;
	if (velXZ.SquaredLength() < kMaxMoveSpeed * kMaxMoveSpeed)
	{
		m_pPlayer->m_vel += moveVec;
	}
	else
	{
		// 水平移動速度が上限を超えていたらその値で固定する
		velXZ.Normalize();
		velXZ *= kMaxMoveSpeed;
		m_pPlayer->m_vel.x = 0.0f;
		m_pPlayer->m_vel.z = 0.0f;
		m_pPlayer->m_vel += velXZ;
	}
	//位置に速度を足す
	m_pPlayer->m_pos += m_pPlayer->m_vel;

}

void PlayerStateMove::Exit()
{}
