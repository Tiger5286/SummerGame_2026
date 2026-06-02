#include "ZombieStateAttack.h"
#include "Zombie.h"
#include "../../Player/Player.h"

#include "ZombieStateIdle.h"
#include "ZombieStateMove.h"

namespace
{
	const std::wstring kAttackAnimName = L"Zombie|Attack";
}

void ZombieStateAttack::Enter(std::weak_ptr<Zombie> pZombie)
{
	m_pZombie = pZombie;
	auto zombie = m_pZombie.lock();
	zombie->m_anim.ChangeAnim(kAttackAnimName,0.5f,false);
	// プレイヤーの方を向く
	Vector3 toPlayerVec = m_pZombie.lock()->m_pPlayer->GetPos() - m_pZombie.lock()->m_pos;
	float toPlayerAngle = toPlayerVec.Angle();
	zombie->m_angle = toPlayerAngle;
}

void ZombieStateAttack::Update()
{
	auto zombie = m_pZombie.lock();
	// 攻撃アニメーションが終わった時
	if (zombie->m_anim.IsEnd())
	{
		// 攻撃の範囲内なら
		if (zombie->IsPlayerInAttackDist())
		{
			// もう一度攻撃する
			zombie->m_anim.ChangeAnim(kAttackAnimName, 0.5f, false);
			// プレイヤーの方を向く
			Vector3 toPlayerVec = m_pZombie.lock()->m_pPlayer->GetPos() - m_pZombie.lock()->m_pos;
			float toPlayerAngle = toPlayerVec.Angle();
			zombie->m_angle = toPlayerAngle;
		}
		else	// 攻撃の範囲外なら
		{
			// プレイヤーの検知範囲内ならmoveステート
			if (zombie->IsPlayerInCircle() || zombie->IsPlayerInFan())
			{
				ChangeState(std::make_shared<ZombieStateMove>());
			}
			else	// 検知範囲外ならidleステート
			{
				ChangeState(std::make_shared<ZombieStateIdle>());
			}
		}
	}
}

void ZombieStateAttack::Exit()
{}
