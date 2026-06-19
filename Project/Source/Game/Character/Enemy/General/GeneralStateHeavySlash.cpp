#include "GeneralStateHeavySlash.h"
#include "General.h"

#include "GeneralStateWalk.h"

namespace
{
	const std::wstring kAnimName = L"General|HeavySlash";

	constexpr int kAttackFrame = 33 * 2;
	constexpr int kStartSlashFrame = 54 * 2;
	constexpr int kEndSlashFrame = 85 * 2;
}

void GeneralStateHeavySlash::Enter(std::weak_ptr<General> pGeneral)
{
	m_pGeneral = pGeneral;
	auto general = m_pGeneral.lock();
	general->m_anim.ChangeAnim(kAnimName, 0.5f, false);
}

void GeneralStateHeavySlash::Update()
{
	auto general = m_pGeneral.lock();

	// アニメーションが終わったらWalk
	if (general->m_anim.IsEnd())
	{
		ChangeState(std::make_shared<GeneralStateWalk>());
		return;
	}
}

void GeneralStateHeavySlash::Exit()
{

}
