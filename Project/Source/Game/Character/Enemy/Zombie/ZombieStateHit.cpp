#include "ZombieStateHit.h"
#include "Zombie.h"

#include "ZombieStateMove.h"

namespace
{
	// アニメーション名
	const std::wstring kHitAnimName = L"Zombie|Hit";
}

void ZombieStateHit::Enter(std::weak_ptr<Character> pOwner)
{
	m_pZombie = std::dynamic_pointer_cast<Zombie>(pOwner.lock());
	m_pZombie.lock()->m_anim.ChangeAnim(kHitAnimName, 0.5f, false);
}

void ZombieStateHit::Update()
{
	// アニメーションが終わったらステートを変更
	if (m_pZombie.lock()->m_anim.IsEnd())
	{
		ChangeState(std::make_shared<ZombieStateMove>());
		return;
	}
}

void ZombieStateHit::Exit()
{}
