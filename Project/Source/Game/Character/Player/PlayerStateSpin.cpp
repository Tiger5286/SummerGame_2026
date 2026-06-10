#include "PlayerStateSpin.h"
#include "Player.h"
#include "../Attack.h"
#include "Wing/SpinWing.h"
#include "Utility/Matrix4x4.h"

#include "PlayerStateIdle.h"

namespace
{
	const std::wstring kAnimName = L"Player|SpinAttack";

	constexpr int kAttackFrame[3] = { 40,55,70 };
	constexpr int kEndAttackFrame = 100;

	const Attack::Data kAttackData = {
		.colliderRadius = 100.0f,
		.damage = 70,
		.hitCharacterType = Character::Type::Enemy
	};
}

void PlayerStateSpin::Enter(std::weak_ptr<Player> pPlayer)
{
	m_pPlayer = pPlayer;
	auto player = m_pPlayer.lock();
	player->m_anim.ChangeAnim(kAnimName, 0.5f, false);

	m_pWing = std::make_shared<SpinWing>();
	m_pWing->Init(player->m_pos + Vector3(0,100,0));

	m_angleVel = -0.3f;
}

void PlayerStateSpin::Update()
{
	m_frame++;

	auto player = m_pPlayer.lock();

	m_pWing->Update();

	m_angleVel += 0.01f;
	if (m_angleVel > 0.3f)
	{
		m_angleVel = 0.3f;
	}
	m_angle += m_angleVel;

	// 攻撃の生成
	if (m_frame == kAttackFrame[0] || m_frame == kAttackFrame[1] || m_frame == kAttackFrame[2])
	{
		m_pAttackL = std::make_shared<Attack>();
		m_pAttackL->SetData(kAttackData);
		m_pAttackL->Init();
		m_pAttackR = std::make_shared<Attack>();
		m_pAttackR->SetData(kAttackData);
		m_pAttackR->Init();
	}
	// 攻撃の更新
	if (m_pAttackL != nullptr)
	{
		Vector3 offset = Vector3(-130, 100, 0) * Matrix4x4::GetRotY(m_angle);
		m_pAttackL->SetPos(player->m_pos + offset);
		m_pAttackL->Update();
	}
	if (m_pAttackR != nullptr)
	{
		Vector3 offset = Vector3(130, 100, 0) * Matrix4x4::GetRotY(m_angle);
		m_pAttackR->SetPos(player->m_pos + offset);
		m_pAttackR->Update();
	}
	// 攻撃の削除
	if (m_frame > kEndAttackFrame)
	{
		m_pAttackL = nullptr;
		m_pAttackR = nullptr;
	}

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
#ifdef _DEBUG
	if (m_pAttackL != nullptr)
	{
		m_pAttackL->Draw();
	}
	if (m_pAttackR != nullptr)
	{
		m_pAttackR->Draw();
	}
#endif
}
