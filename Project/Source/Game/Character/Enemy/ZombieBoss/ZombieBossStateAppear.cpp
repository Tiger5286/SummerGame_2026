#include "ZombieBossStateAppear.h"
#include "Singleton/EffectManager.h"

#include "ZombieBossStateIdle.h"

namespace
{
	constexpr const wchar_t* kIdleAnimName = L"ZombieBoss|Idle";
	constexpr const wchar_t* kScreamAnimName = L"ZombieBoss|Screem";

	constexpr int kStartAnimFrame = 120;
	constexpr float kAnimSpeed = 0.3f;
	constexpr int kStartScreamFrame = kStartAnimFrame + 30 * 2 * (kAnimSpeed + 1.0f);

	const Vector3 kEffectOffset = Vector3(0, 150, 0);
}

void ZombieBossStateAppear::OnEnter()
{
	m_pZombieBoss = std::dynamic_pointer_cast<ZombieBoss>(m_pOwner.lock());
	auto zombieBoss = m_pZombieBoss.lock();
	zombieBoss->m_anim.ChangeAnim(kIdleAnimName, MyLib::kDefaultAnimSpeed, false);
}

void ZombieBossStateAppear::Update()
{
	auto zombieBoss = m_pZombieBoss.lock();
	m_frame++;
	if (m_frame == kStartAnimFrame)
	{
		zombieBoss->m_anim.ChangeAnim(kScreamAnimName, kAnimSpeed, false);
	}
	if (m_frame == kStartScreamFrame)
	{
		EffectManager::GetInstance().PlayEffect(L"BossRoar", zombieBoss->m_pos + kEffectOffset);
	}
	if (zombieBoss->m_anim == kScreamAnimName)
	{
		if (zombieBoss->m_anim.IsEnd())
		{
			ChangeState(std::make_shared<ZombieBossStateIdle>());
			return;
		}
	}
}

void ZombieBossStateAppear::Exit()
{
	auto zombieBoss = m_pZombieBoss.lock();
	zombieBoss->m_attackCooltime = ZombieBoss::kAttackCooltime;
}
