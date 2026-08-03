#include "ZombieBossStateAttack.h"
#include "Game/Character/Attack.h"

#include "ZombieBossStateIdle.h"

namespace
{
	constexpr const wchar_t* kAnimName = L"ZombieBoss|Attack";

	constexpr int kStartAttackFrame = 28 * 2;
	constexpr int kEndAttackFrame = 38 * 2;

	const Vector3 kColliderOffset = Vector3(0, 100, -100);
	const MyLib::AttackData kAttackData = {
		120.0f,
		80,
		MyLib::CharacterType::Player
	};
}

void ZombieBossStateAttack::OnEnter()
{
	m_pZombieBoss = std::dynamic_pointer_cast<ZombieBoss>(m_pOwner.lock());
	auto zombieBoss = m_pZombieBoss.lock();
	zombieBoss->m_anim.ChangeAnim(kAnimName, MyLib::kDefaultAnimSpeed, false);
}

void ZombieBossStateAttack::Update()
{
	m_frame++;

	auto zombieBoss = m_pZombieBoss.lock();

	// 当たり判定開始
	if (m_frame == kStartAttackFrame)
	{
		m_pAtk = std::make_shared<Attack>();
		m_pAtk->SetData(kAttackData, shared_from_this());
		m_pAtk->Init();
	}
	// 当たり判定終了
	if (m_frame == kEndAttackFrame)
	{
		m_pAtk = nullptr;
	}
	// 当たり判定の移動
	if (m_pAtk != nullptr)
	{
		Vector3 colPos = zombieBoss->m_pos + (kColliderOffset * Matrix4x4::GetRotY(zombieBoss->m_angle));
		m_pAtk->SetPos(colPos);
		m_pAtk->Update();
	}

	if (zombieBoss->m_anim.IsEnd())
	{
		ChangeState(std::make_shared<ZombieBossStateIdle>());
	}
}

void ZombieBossStateAttack::Exit()
{
	auto zombieBoss = m_pZombieBoss.lock();
	zombieBoss->m_attackCooltime = ZombieBoss::kAttackCooltime;
}

void ZombieBossStateAttack::Draw()
{
#ifdef _DEBUG
	if (m_pAtk != nullptr)
	{
		m_pAtk->Draw();
	}
#endif
}