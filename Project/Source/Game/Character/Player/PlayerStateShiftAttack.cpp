#include "PlayerStateShiftAttack.h"
#include "Player.h"
#include "Singleton/Input.h"

#include "PlayerStateMove.h"
#include "PlayerStateIdle.h"

namespace
{

}

void PlayerStateShiftAttack::OnEnter()
{
	m_pPlayer = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	auto player = m_pPlayer.lock();
	player->m_anim.ChangeAnim(L"Player|ShiftAttack", MyLib::kDefaultAnimSpeed, false);
}

void PlayerStateShiftAttack::Update()
{
	auto player = m_pPlayer.lock();
	auto& input = Input::GetInstance();

	player->m_vel.y = 0.0f;

	// アニメーションが終わったらステート遷移
	if (player->m_anim.IsEnd())
	{
		// 移動していたらmoveへ
		if (input.GetStickInput(MyLib::LR::Left).SquaredLength() > 0.0f)
		{
			ChangeState(std::make_shared<PlayerStateMove>());
			return;
		}
		// そうでないならidleへ
		ChangeState(std::make_shared<PlayerStateIdle>());
		return;
	}
}

void PlayerStateShiftAttack::Exit()
{
}
