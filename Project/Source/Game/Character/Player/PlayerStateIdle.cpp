#include "PlayerStateIdle.h"
#include "Player.h"
#include "Singleton/Input.h"

#include "PlayerStateMove.h"
#include "PlayerStateFall.h"

namespace
{
	// アニメーション名
	const std::wstring kIdleAnimName = L"Player|Idle";
}

void PlayerStateIdle::Enter(std::shared_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_pPlayer->m_anim.ChangeAnim(kIdleAnimName);
}

void PlayerStateIdle::Update()
{
	// 入力を取得
	auto& input = Input::GetInstance();
	// スティック入力があったらMoveへ
	if (input.GetStickInput(LR::Left).SquaredLength() > 0.0f)
	{
		ChangeState(std::make_shared<PlayerStateMove>());
		return;
	}
	// 接地していなかったらFallにする
	if (!(m_pPlayer->m_isGround))
	{
		ChangeState(std::make_shared<PlayerStateFall>());
		return;
	}
	// ジャンプ処理
	m_pPlayer->Jump();
}

void PlayerStateIdle::Exit()
{

}
