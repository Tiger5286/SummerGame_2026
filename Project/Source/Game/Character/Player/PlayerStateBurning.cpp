#include "PlayerStateBurning.h"
#include "Player.h"
#include "../../Camera/Camera.h"
#include "../../Camera/CameraStateBurning.h"
#include "../../Camera/CameraStateFree.h"

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
	player->m_pCamera.lock()->ChangeState(std::make_shared<CameraStateBurning>());
	player->RotateToTarget(99999.9f);
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
	m_pPlayer.lock()->m_pCamera.lock()->ChangeState(std::make_shared<CameraStateFree>());
}
