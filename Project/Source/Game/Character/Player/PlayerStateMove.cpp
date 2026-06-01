#include "PlayerStateMove.h"
#include "Player.h"
#include "Singleton/Input.h"
#include "Utility/Matrix4x4.h"

#include "PlayerStateIdle.h"
#include "PlayerStateFall.h"
#include "PlayerStateDodge.h"

namespace
{
	// アニメーション名
	const std::wstring kRunAnimName = L"Player|Run";
}

void PlayerStateMove::Enter(std::shared_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_pPlayer->m_anim.ChangeAnim(kRunAnimName);
}

void PlayerStateMove::Update()
{
	// 移動処理
	m_pPlayer->Move();
	// ジャンプ処理
	m_pPlayer->Jump();

	// 入力を取得
	auto& input = Input::GetInstance();
	// 回避が入力されたら回避
	if (input.IsTriggerd(XINPUT_BUTTON_B))
	{
		ChangeState(std::make_shared<PlayerStateDodge>());
		return;
	}
	// 接地していなかったらFallにする
	if (!(m_pPlayer->m_isGround))
	{
		ChangeState(std::make_shared<PlayerStateFall>());
		return;
	}
	// xz速度が0になったらidle
	auto velXZ = m_pPlayer->m_vel;
	velXZ.y = 0.0f;
	if (velXZ.SquaredLength() <= 0.0f)
	{
		ChangeState(std::make_shared<PlayerStateIdle>());
		return;
	}
}

void PlayerStateMove::Exit()
{}
