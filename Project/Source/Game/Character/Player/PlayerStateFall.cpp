#include "PlayerStateFall.h"
#include "Player.h"
#include "Singleton/Input.h"

#include "PlayerStateIdle.h"
#include "PlayerStateMove.h"
#include "PlayerStateDodge.h"
#include "PlayerStateAttack.h"
#include "PlayerStateShift.h"
#include "PlayerStateSpin.h"
#include "PlayerStateBurning.h"

namespace
{
	// アニメーション名
	const std::wstring kFallAnimName = L"Player|Fall";
	const std::wstring kJumpAnimName = L"Player|Jump";
}

void PlayerStateFall::OnEnter()
{
	m_pPlayer = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	auto player = m_pPlayer.lock();
	if (player->m_vel.y > 0.0f)
	{
		player->m_anim.ChangeAnim(kJumpAnimName, MyLib::kDefaultAnimSpeed, false);
		m_isEnterJump = true;
	}
	else
	{
		player->m_anim.ChangeAnim(kFallAnimName, MyLib::kDefaultAnimSpeed, false);
	}
}

void PlayerStateFall::Update()
{
	auto player = m_pPlayer.lock();

	if (player->m_vel.y < 0.0f && m_isEnterJump)
	{
		player->m_anim.ChangeAnim(kFallAnimName, MyLib::kDefaultAnimSpeed,false);
		m_isEnterJump = false;
	}

	// 移動処理
	player->Move();

	// 入力を取得
	auto& input = Input::GetInstance();
	// バーニングを入力していたらバーニング
	if (input.IsTriggerd(player->kBurning) && player->GetSpecialCharge() >= player->kMaxSpecialCharge)
	{
		ChangeState(std::make_shared<PlayerStateBurning>());
		return;
	}
	// 回避ボタンを押したら回避
	if (input.IsTriggerd(player->kDodge))
	{
		ChangeState(std::make_shared<PlayerStateDodge>());
		return;
	}
	// 攻撃を入力していたら、かつ空中で攻撃が発動可能なら攻撃
	if (input.IsTriggerd(player->kAttack) && player->m_isCanAirAttack)
	{
		ChangeState(std::make_shared<PlayerStateAttack>());
		return;
	}
	// シフトを入力していたらシフト
	if (input.IsTriggerd(player->kShift) && player->m_isCanAirShift)
	{
		ChangeState(std::make_shared<PlayerStateShift>());
		return;
	}
	// スピンを入力していたらスピン
	if (input.IsTriggerd(player->kSpin) && player->m_skillCooltime >= player->kSkillCooltime)
	{
		ChangeState(std::make_shared<PlayerStateSpin>());
		return;
	}

	// 接地していたら
	if (player->m_isGround)
	{
		// yを除いた速度ベクトルを計算
		Vector3 velXZ = player->m_vel;
		velXZ.y = 0.0f;
		// 移動しているならmoveへ
		if (velXZ.SquaredLength() > 0.0f)
		{
			ChangeState(std::make_shared<PlayerStateMove>());
			return;
		}
		// 移動していないならidleへ
		if (velXZ.SquaredLength() <= 0.0f)
		{
			ChangeState(std::make_shared<PlayerStateIdle>());
			return;
		}
	}
}

void PlayerStateFall::Exit()
{

}
