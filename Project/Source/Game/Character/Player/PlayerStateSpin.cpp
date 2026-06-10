#include "PlayerStateSpin.h"
#include "Player.h"

#include "PlayerStateIdle.h"
#include "Wing/SpinWing.h"

namespace
{
	const std::wstring kAnimName = L"Player|SpinAttack";
}

void PlayerStateSpin::Enter(std::weak_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	auto player = m_pPlayer.lock();
	player->m_anim.ChangeAnim(kAnimName, 0.5f, false);

	m_pWing = std::make_shared<SpinWing>();
	m_pWing->Init(player->m_pos + Vector3(0,100,0));
}

void PlayerStateSpin::Update()
{
	auto player = m_pPlayer.lock();

	m_pWing->Update();

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

void PlayerStateSpin::Draw()
{
	m_pWing->Draw();
}
