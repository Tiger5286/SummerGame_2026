#include "PlayerStateBurning.h"
#include "Player.h"
#include "../../Camera/Camera.h"
#include "../../Camera/CameraStateBurning.h"
#include "../../Camera/CameraStateFree.h"
#include "../Attack.h"
#include "Singleton/EffectManager.h"
#include "BurningWing.h"

#include "PlayerStateIdle.h"

namespace
{
	const std::wstring kBurningAnimName = L"Player|Power";

	constexpr int kStartAttackFrame = 120;
	constexpr int kEndAttackFrame = 300;

	constexpr float kAttackPerSecond = 0.25f;

	const Attack::Data kAttackData = {
		1500.0f,
		30,
		Character::Type::Enemy
	};
}

void PlayerStateBurning::Enter(std::weak_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	auto player = m_pPlayer.lock();
	player->m_anim.ChangeAnim(kBurningAnimName, 0.5f, false);
	player->m_pCamera.lock()->ChangeState(std::make_shared<CameraStateBurning>());
	player->RotateToTarget(FLT_MAX);
	EffectManager::GetInstance().PlayEffect(L"Burning", player->m_pos + Vector3(0, 100, 0));

	m_pWing = std::make_shared<BurningWing>();
	m_pWing->Init(player->m_pos, player->m_angle);
}

void PlayerStateBurning::Update()
{
	m_frame++;
	auto player = m_pPlayer.lock();
	m_pWing->Update();

	if (m_frame > kStartAttackFrame && m_frame < kEndAttackFrame)
	{
		if (m_frame % static_cast<int>(60 * kAttackPerSecond) == 0)
		{
			//EffectManager::GetInstance().PlayEffect(L"Distortion", player->m_pos + Vector3::Up() * 100);
			m_pAtk = std::make_shared<Attack>();
			m_pAtk->SetData(kAttackData);
			m_pAtk->Init();
			m_pAtk->SetPos(player->m_pos);
		}
	}
	else
	{
		m_pAtk = nullptr;
	}

	if (m_pAtk != nullptr)
	{
		m_pAtk->Update();
	}

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

void PlayerStateBurning::Draw()
{
	m_pWing->Draw();
#ifdef _DEBUG
	if (m_pAtk != nullptr)
	{
		m_pAtk->Draw();
	}
#endif
}
