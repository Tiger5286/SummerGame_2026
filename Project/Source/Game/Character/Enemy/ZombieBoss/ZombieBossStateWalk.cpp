#include "ZombieBossStateWalk.h"
#include "Game/Character/Player/Player.h"
#include "Utility/MyLib.h"

#include "ZombieBossStateIdle.h"

namespace
{
	constexpr const wchar_t* kAnimName = L"ZombieBoss|Walk";

	constexpr float kMoveSpeed = 1.0f;

	constexpr float kStopMoveDist = 200.0f;
}

void ZombieBossStateWalk::OnEnter()
{
	m_pZombieBoss = std::dynamic_pointer_cast<ZombieBoss>(m_pOwner.lock());
	auto zombieBoss = m_pZombieBoss.lock();
	zombieBoss->m_anim.ChangeAnim(kAnimName);
}

void ZombieBossStateWalk::Update()
{
	auto zombieBoss = m_pZombieBoss.lock();
	auto player = zombieBoss->m_pPlayer;

	// プレイヤーへのベクトル
	Vector3 toPlayer = player->GetPos() - zombieBoss->m_pos;
	// プレイヤーの方を向く
	float angle = MyLib::GetAngleVec(toPlayer.z, toPlayer.x);
	zombieBoss->m_angle = angle;
	// プレイヤーの方に移動する
	zombieBoss->m_vel = toPlayer.Normalized() * kMoveSpeed;

	// 一定距離以内に近づいたら待機ステートに遷移する
	if (toPlayer.SquaredLength() < kStopMoveDist * kStopMoveDist)
	{
		ChangeState(std::make_shared<ZombieBossStateIdle>());
		return;
	}
}

void ZombieBossStateWalk::Exit()
{
}
