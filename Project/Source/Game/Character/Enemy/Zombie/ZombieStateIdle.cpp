#include "ZombieStateIdle.h"
#include "Zombie.h"	

#include "ZombieStateMove.h"

namespace
{
	const std::wstring kIdleAnimName = L"Zombie|Idle";
}

void ZombieStateIdle::Enter(std::weak_ptr<Zombie> pZombie)
{
	m_pZombie = pZombie;
	m_pZombie.lock()->m_anim.ChangeAnim(kIdleAnimName);
	m_pZombie.lock()->m_isFighting = false;
}

void ZombieStateIdle::Update()
{
	// プレイヤーを見つけたら移動ステートに切り替える
	if (m_pZombie.lock()->IsPlayerInFan() || m_pZombie.lock()->IsPlayerInCircle())
	{
		ChangeState(std::make_shared<ZombieStateMove>());
	}
}

void ZombieStateIdle::Exit()
{}
