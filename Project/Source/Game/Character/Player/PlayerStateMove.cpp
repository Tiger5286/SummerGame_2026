#include "PlayerStateMove.h"
#include "Player.h"

namespace
{
	// アニメーション名
	const std::wstring kRunAnimName = L"Player|Run";
}

void PlayerStateMove::Enter(std::shared_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	m_pPlayer->m_anim.ChangeAnim(kRunAnimName);
}

void PlayerStateMove::Update()
{

}

void PlayerStateMove::Exit()
{}
