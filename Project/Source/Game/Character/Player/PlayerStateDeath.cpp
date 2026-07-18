#include "PlayerStateDeath.h"
#include "Player.h"

void PlayerStateDeath::OnEnter()
{
	m_pPlayer = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	auto player = m_pPlayer.lock();
	player->m_anim.ChangeAnim(L"Player|Death", 0.5f, false);
	player->m_isCanHitAttack = false;
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
