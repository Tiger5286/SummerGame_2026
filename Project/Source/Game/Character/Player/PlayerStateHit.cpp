#include "PlayerStateHit.h"
#include "Player.h"	

#include "PlayerStateIdle.h"

namespace
{
	const std::wstring kHitAnimName = L"Player|Hit";
}

void PlayerStateHit::Enter(std::weak_ptr<Character> pOwner)
{
	m_pPlayer = std::dynamic_pointer_cast<Player>(pOwner.lock());
	m_pPlayer.lock()->m_anim.ChangeAnim(kHitAnimName, 0.5f, false);
}

void PlayerStateHit::Update()
{
	// アニメーションが終わったらステート遷移
	if (m_pPlayer.lock()->m_anim.IsEnd())
	{
		ChangeState(std::make_shared<PlayerStateIdle>());
		return;
	}
}

void PlayerStateHit::Exit()
{}
