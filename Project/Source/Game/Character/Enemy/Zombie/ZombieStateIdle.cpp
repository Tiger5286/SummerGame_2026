#include "ZombieStateIdle.h"
#include "Zombie.h"	

namespace
{
	const std::wstring kIdleAnimName = L"Zombie|Idle";
}

void ZombieStateIdle::Enter(std::weak_ptr<Zombie> pZombie)
{
	m_pZombie = pZombie;
	m_pZombie.lock()->m_anim.ChangeAnim(kIdleAnimName);
}

void ZombieStateIdle::Update()
{}

void ZombieStateIdle::Exit()
{}
