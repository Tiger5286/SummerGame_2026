#include "PlayerStateFall.h"
#include "Player.h"
#include "Singleton/Input.h"

#include "PlayerStateIdle.h"
#include "PlayerStateMove.h"
#include "PlayerStateDodge.h"
#include "PlayerStateAttack.h"
#include "PlayerStateShift.h"

namespace
{
	// アニメーション名
	const std::wstring kFallAnimName = L"Player|Fall";
}

void PlayerStateFall::Enter(std::weak_ptr<Character> pOwner)
{
	m_pPlayer = std::dynamic_pointer_cast<Player>(pOwner.lock());
	m_pPlayer.lock()->m_anim.ChangeAnim(kFallAnimName);
}

void PlayerStateFall::Update()
{
	auto player = m_pPlayer.lock();

	// 移動処理
	player->Move();

	// 入力を取得
	auto& input = Input::GetInstance();
	// 回避ボタンを押したら回避
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
	if (input.IsTriggerd(player->kShift) && player->m_isCanAirShift)
	{
		ChangeState(std::make_shared<PlayerStateShift>());
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
