#include "ZombieBossStateDeath.h"

namespace
{
	constexpr const wchar_t* kAnimName = L"ZombieBoss|Death";
}

void ZombieBossStateDeath::OnEnter()
{
	m_pZombieBoss = std::dynamic_pointer_cast<ZombieBoss>(GetOwner().lock());
	auto zombieBoss = m_pZombieBoss.lock();
	zombieBoss->m_anim.ChangeAnim(kAnimName, MyLib::kDefaultAnimSpeed, false);
	zombieBoss->m_isDying = true;
	zombieBoss->m_isCanHitAttack = false;
}

void ZombieBossStateDeath::Update()
{
	auto zombieBoss = m_pZombieBoss.lock();
	if (zombieBoss->m_anim.IsEnd())
	{
		zombieBoss->m_isDead = true;
	}
}

void ZombieBossStateDeath::Exit()
{
}
