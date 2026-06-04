#include "PlayerStateIdle.h"
#include "Player.h"
#include "Singleton/Input.h"

#include "PlayerStateMove.h"
#include "PlayerStateFall.h"
#include "PlayerStateDodge.h"
#include "PlayerStateAttack.h"
#include "PlayerStateShift.h"

namespace
{
	// アニメーション名
	const std::wstring kIdleAnimName = L"Player|Idle";
}

void PlayerStateIdle::Enter(std::weak_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_pPlayer.lock()->m_anim.ChangeAnim(kIdleAnimName);
}

void PlayerStateIdle::Update()
{
	// ジャンプ処理
	m_pPlayer.lock()->Jump();

	// 入力を取得
	auto& input = Input::GetInstance();
	// 回避を入力していたら回避
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
	// スティック入力があったらMoveへ
	if (input.GetStickInput(LR::Left).SquaredLength() > 0.0f)
	{
		ChangeState(std::make_shared<PlayerStateMove>());
		return;
	}
	// 接地していなかったらFallにする
	if (!(m_pPlayer.lock()->m_isGround))
	{
		ChangeState(std::make_shared<PlayerStateFall>());
		return;
	}
}

void PlayerStateIdle::Exit()
{

}
