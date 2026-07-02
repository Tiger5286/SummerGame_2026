#include "PlayerStateIdle.h"
#include "Player.h"
#include "Singleton/Input.h"

#include "PlayerStateMove.h"
#include "PlayerStateFall.h"
#include "PlayerStateDodge.h"
#include "PlayerStateAttack.h"
#include "PlayerStateShift.h"
#include "PlayerStateBurning.h"
#include "PlayerStateSpin.h"

namespace
{
	// アニメーション名
	const std::wstring kIdleAnimName = L"Player|Idle";
}

void PlayerStateIdle::Enter(std::weak_ptr<Character> pOwner)
{
	m_pPlayer = std::dynamic_pointer_cast<Player>(pOwner.lock());
	m_pPlayer.lock()->m_anim.ChangeAnim(kIdleAnimName);
}

void PlayerStateIdle::Update()
{
	auto player = m_pPlayer.lock();

	// ジャンプ処理
	player->Jump();

	// 入力を取得
	auto& input = Input::GetInstance();

	// バーニングを入力していたらバーニング
	if (input.IsTriggerd(player->kBurning) && player->GetSpecialCharge() >= 1000)
	{
		ChangeState(std::make_shared<PlayerStateBurning>());
		return;
	}
	// スピンを入力したいたらスピン
	if (input.IsTriggerd(player->kSpin))
	{
		ChangeState(std::make_shared<PlayerStateSpin>());
		return;
	}
	// 回避を入力していたら回避
	if (input.IsTriggerd(player->kDodge))
	{
		ChangeState(std::make_shared<PlayerStateDodge>());
		return;
	}
	// 攻撃を入力していたら攻撃
	if (input.IsTriggerd(player->kAttack))
	{
		ChangeState(std::make_shared<PlayerStateAttack>());
		return;
	}
	// シフトを入力していたらシフト
	if (input.IsTriggerd(player->kShift))
	{
		ChangeState(std::make_shared<PlayerStateShift>());
		return;
	}
	// スティック入力があったらMoveへ
	if (input.GetStickInput(MyLib::LR::Left).SquaredLength() > 0.0f)
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
