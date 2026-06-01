#include "PlayerStateMove.h"
#include "Player.h"
#include "Singleton/Input.h"
#include "Utility/Matrix4x4.h"

#include "PlayerStateIdle.h"
#include "PlayerStateFall.h"

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
	// 入力を取得
	auto& input = Input::GetInstance();
	// スティック入力がなくなったらIdleにする
	if (input.GetStickInput(LR::Left).SquaredLength() <= 0.0f)
	{
		ChangeState(std::make_shared<PlayerStateIdle>());
		return;
	}
	// 接地していなかったらFallにする
	if (!(m_pPlayer->m_isGround))
	{
		ChangeState(std::make_shared<PlayerStateFall>());
		return;
	}
	// 移動処理
	m_pPlayer->Move();
	// ジャンプ処理
	m_pPlayer->Jump();
}

void PlayerStateMove::Exit()
{}
