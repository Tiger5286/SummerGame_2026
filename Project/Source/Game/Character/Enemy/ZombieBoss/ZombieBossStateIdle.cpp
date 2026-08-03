#include "ZombieBossStateIdle.h"
#include "Game/Character/Player/Player.h"

#include "ZombieBossStateWalk.h"
#include "ZombieBossStateAttack.h"

namespace
{
	constexpr const wchar_t* kAnimName = L"ZombieBoss|Idle";

	constexpr float kIdleDist = 200.0f;
}

void ZombieBossStateIdle::OnEnter()
{
	m_pZombieBoss = std::dynamic_pointer_cast<ZombieBoss>(m_pOwner.lock());
	auto zombieBoss = m_pZombieBoss.lock();
	zombieBoss->m_anim.ChangeAnim(kAnimName);
}

void ZombieBossStateIdle::Update()
{
	auto zombieBoss = m_pZombieBoss.lock();
	auto player = zombieBoss->m_pPlayer;
	// プレイヤーへのベクトル
	Vector3 toPlayer = player->GetPos() - zombieBoss->m_pos;
	// プレイヤーの方を向く
	float angle = MyLib::GetAngleVec(toPlayer.z, toPlayer.x);
	zombieBoss->m_angle = angle;
	// 条件を満たしたら攻撃ステートに遷移する
	bool isEndCooltime = zombieBoss->m_attackCooltime <= 0;	// 攻撃クールタイムが終わっているかどうか
	bool isNearPlayer = toPlayer.SquaredLength() <= kIdleDist * kIdleDist;	// プレイヤーが一定距離以内にいるかどうか
	if (isEndCooltime && isNearPlayer)
	{
		zombieBoss->RandomAttack();
		return;
	}
	// 一定距離以上離れていたら歩きステートに遷移する
	if (toPlayer.SquaredLength() > kIdleDist * kIdleDist)
	{
		ChangeState(std::make_shared<ZombieBossStateWalk>());
		return;
	}
}

void ZombieBossStateIdle::Exit()
{
}
