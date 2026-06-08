#include "PlayerStateBurning.h"
#include "Player.h"

#include "PlayerStateIdle.h"

namespace
{
	const std::wstring kBurningAnimName = L"Player|Power";
}

void PlayerStateBurning::Enter(std::weak_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	auto player = m_pPlayer.lock();
	player->m_anim.ChangeAnim(kBurningAnimName, 0.5f, false);
}

void PlayerStateBurning::Update()
{
	auto player = m_pPlayer.lock();
	if (player->m_anim.IsEnd())
	{
		ChangeState(std::make_shared<PlayerStateIdle>());
		return;
	}
}

void PlayerStateBurning::Exit()
{

}
