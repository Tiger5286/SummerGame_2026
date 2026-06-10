#include "PlayerStateSpin.h"
#include "Player.h"

#include "PlayerStateIdle.h"

namespace
{
	const std::wstring kAnimName = L"Player|SpinAttack";
}

void PlayerStateSpin::Enter(std::weak_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	auto player = m_pPlayer.lock();
	player->m_anim.ChangeAnim(kAnimName, 0.5f, false);
}

void PlayerStateSpin::Update()
{
	auto player = m_pPlayer.lock();
	// アニメーションが終わったらIdle
	if (player->m_anim.IsEnd())
	{
		ChangeState(std::make_shared<PlayerStateIdle>());
		return;
	}
}

void PlayerStateSpin::Exit()
{

}
