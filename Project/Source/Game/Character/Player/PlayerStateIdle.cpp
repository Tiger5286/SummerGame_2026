#include "PlayerStateIdle.h"
#include "Player.h"

namespace
{
	// アニメーション名
	const std::wstring kIdleAnimName = L"Player|Idle";
}

void PlayerStateIdle::Enter(std::shared_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_pPlayer->m_anim.ChangeAnim(kIdleAnimName);
}

void PlayerStateIdle::Update()
{

}

void PlayerStateIdle::Exit()
{

}
