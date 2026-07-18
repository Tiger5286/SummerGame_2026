#include "ZombieStateDeath.h"
#include "Zombie.h"

namespace
{
	const std::wstring kDeathAnimName = L"Zombie|Death";
}

void ZombieStateDeath::OnEnter()
{
	m_pZombie = std::dynamic_pointer_cast<Zombie>(m_pOwner.lock());
	m_pZombie.lock()->m_anim.ChangeAnim(kDeathAnimName, 0.5f, false);
	m_pZombie.lock()->m_isDying = true;
	m_pZombie.lock()->m_isCanHitAttack = false;
}

void ZombieStateDeath::Update()
{
	auto zombie = m_pZombie.lock();
	// アニメーションが終わったら死亡フラグを付ける
	if (zombie->m_anim.IsEnd())
	{
		zombie->m_isDead = true;
	}
}

void ZombieStateDeath::Exit()
{}
