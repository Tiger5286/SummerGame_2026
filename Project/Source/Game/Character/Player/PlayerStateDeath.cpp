#include "PlayerStateDeath.h"
#include "Player.h"

void PlayerStateDeath::Enter(std::weak_ptr<Character> pOwner)
{
	m_pPlayer = std::dynamic_pointer_cast<Player>(pOwner.lock());
	auto player = m_pPlayer.lock();
	player->m_anim.ChangeAnim(L"Player|Death", 0.5f, false);
}

void PlayerStateDeath::Update()
{
	auto player = m_pPlayer.lock();
	if (player->m_anim.IsEnd())
	{
		player->m_isDead = true;
	}
}

void PlayerStateDeath::Exit()
{
}
