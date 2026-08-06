#include "PlayerStateHit.h"
#include "Player.h"
#include "Singleton/SoundManager.h"

#include "PlayerStateIdle.h"

namespace
{
	const std::wstring kHitAnimName = L"Player|Hit";
}

void PlayerStateHit::OnEnter()
{
	m_pPlayer = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	m_pPlayer.lock()->m_anim.ChangeAnim(kHitAnimName, 0.5f, false);
	SoundManager::GetInstance().PlaySoundGame(L"Damage");
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
