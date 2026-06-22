#include "GeneralStateIdle.h"
#include "General.h"
#include "../../Player/Player.h"

namespace
{
	const std::wstring kAnimName = L"General|Idle";

	constexpr float kWalkDist = 500.0f;
}

void GeneralStateIdle::Enter(std::weak_ptr<General> pGeneral)
{
	m_pGeneral = pGeneral;
	auto general = m_pGeneral.lock();
	general->m_anim.ChangeAnim(kAnimName);
}

void GeneralStateIdle::Update()
{
	auto general = m_pGeneral.lock();
	auto player = general->m_pPlayer;

	// プレイヤーとの距離が一定以上なら歩く
	
}

void GeneralStateIdle::Exit()
{

}
