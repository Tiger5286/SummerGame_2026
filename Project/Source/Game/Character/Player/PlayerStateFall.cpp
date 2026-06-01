#include "PlayerStateFall.h"
#include "Player.h"
#include "Singleton/Input.h"

#include "PlayerStateIdle.h"
#include "PlayerStateMove.h"
#include "PlayerStateDodge.h"
#include "PlayerStateAttack.h"

namespace
{
	// アニメーション名
	const std::wstring kFallAnimName = L"Player|Fall";
}

void PlayerStateFall::Enter(std::shared_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_pPlayer->m_anim.ChangeAnim(kFallAnimName);
}

void PlayerStateFall::Update()
{
	// 移動処理
	m_pPlayer->Move();

	// 入力を取得
	auto& input = Input::GetInstance();
	// 回避ボタンを押したら回避
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
	// 接地していたら
	if (m_pPlayer->m_isGround)
	{
		// yを除いた速度ベクトルを計算
		Vector3 velXZ = m_pPlayer->m_vel;
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
