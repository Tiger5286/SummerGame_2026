#include "ZombieBossStateIdle.h"
#include "Game/Character/Player/Player.h"

#include "ZombieBossStateWalk.h"

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
