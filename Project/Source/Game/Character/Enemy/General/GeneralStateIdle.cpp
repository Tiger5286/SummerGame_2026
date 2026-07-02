#include "GeneralStateIdle.h"
#include "General.h"
#include "../../Player/Player.h"

#include "GeneralStateWalk.h"

namespace
{
	const std::wstring kAnimName = L"General|Idle";

	constexpr float kWalkDist = 500.0f;
}

void GeneralStateIdle::Enter(std::weak_ptr<Character> pOwner)
{
	m_pGeneral = std::dynamic_pointer_cast<General>(pOwner.lock());
	auto general = m_pGeneral.lock();
	general->m_anim.ChangeAnim(kAnimName);
}

void GeneralStateIdle::Update()
{
	auto general = m_pGeneral.lock();
	auto player = general->m_pPlayer;

	// クールタイムを減らす
	general->m_attackCooltime--;

	// クールタイムが終わっていたら攻撃
	if (general->m_attackCooltime < 0)
	{
		general->AttackRandom();
		return;
	}

	// プレイヤーとの距離が一定以上なら歩く
	Vector3 toPlayer = player->GetPos() - general->m_pos;
	if (toPlayer.SquaredLength() > kWalkDist * kWalkDist)
	{
		ChangeState(std::make_shared<GeneralStateWalk>());
		return;
	}
	
}

void GeneralStateIdle::Exit()
{

}
