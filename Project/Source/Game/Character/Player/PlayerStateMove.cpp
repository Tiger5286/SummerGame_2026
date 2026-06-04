#include "PlayerStateMove.h"
#include "Player.h"
#include "Singleton/Input.h"
#include "Utility/Matrix4x4.h"

#include "PlayerStateIdle.h"
#include "PlayerStateFall.h"
#include "PlayerStateDodge.h"
#include "PlayerStateAttack.h"
#include "PlayerStateShift.h"

namespace
{
	// アニメーション名
	const std::wstring kRunAnimName = L"Player|Run";
}

void PlayerStateMove::Enter(std::weak_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_pPlayer.lock()->m_anim.ChangeAnim(kRunAnimName);
}

void PlayerStateMove::Update()
{
	// 移動処理
	m_pPlayer.lock()->Move();
	// ジャンプ処理
	m_pPlayer.lock()->Jump();

	// 入力を取得
	auto& input = Input::GetInstance();
	// 回避が入力されたら回避
	if (input.IsTriggerd(XINPUT_BUTTON_B))
	{
		ChangeState(std::make_shared<PlayerStateDodge>());
		return;
	}
	// 攻撃を入力していたら攻撃
	if (input.IsTriggerd(XINPUT_BUTTON_X))
	{
		ChangeState(std::make_shared<PlayerStateAttack>());
		return;
	}
	// シフトを入力していたらシフト
	if (input.IsTriggerd(XINPUT_BUTTON_Y))
	{
		ChangeState(std::make_shared<PlayerStateShift>());
		return;
	}
	// 接地していなかったらFallにする
	if (!(m_pPlayer.lock()->m_isGround))
	{
		ChangeState(std::make_shared<PlayerStateFall>());
		return;
	}
	// xz速度が0になったらidle
	auto velXZ = m_pPlayer.lock()->m_vel;
	velXZ.y = 0.0f;
	if (velXZ.SquaredLength() <= 0.0f)
	{
		ChangeState(std::make_shared<PlayerStateIdle>());
		return;
	}
}

void PlayerStateMove::Exit()
{}
